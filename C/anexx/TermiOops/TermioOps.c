
#include "TermioOps.h"
#include "logs.h"

#include <unistd.h> 
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <errno.h> 

typedef struct {
    const int flag;
    const char *name;
    char status;
} TermiosAttr;


typedef struct {
    const char name[TERMIOS_REG_MAX_NAME];
    struct termios termios_preset;
} TermiosPreset;


#define ITER_ATTR( attr ) for( int attr_idx= 0, f= sizeof( attr ) / sizeof( TermiosAttr ) ; attr_idx < f ; attr_idx++ )


static struct termios Termios_original;
static char termios_original_set= 1;
static TermiosPreset *TermiosCustom= NULL;
static unsigned char Termios_profiles_count= 0;
static char current_termios= 0;
static char previous_termios= 0;

static TermiosAttr lflags[]={
    {ECHO,    "ECHO"},
    {ECHOE,   "ECHOE"},
    {ECHOK,   "ECHOK"},
    {ECHONL,  "ECHONL"},
    {ICANON,  "ICANON"},
    {IEXTEN,  "IEXTEN"},
    {ISIG,    "ISIG"},
    {NOFLSH,  "NOFLSH"},
    {TOSTOP,  "TOSTOP"}
};

static TermiosAttr iflags[]={
    {ECHO,    "ECHO"},
    {ECHOE,   "ECHOE"},
    {ECHOK,   "ECHOK"},
    {ECHONL,  "ECHONL"},
    {ICANON,  "ICANON"},
    {IEXTEN,  "IEXTEN"},
    {ISIG,    "ISIG"},
    {NOFLSH,  "NOFLSH"},
    {TOSTOP,  "TOSTOP"}
};

static TermiosAttr oflags[]={
    {OPOST,   "OPOST"},
    {ONLCR,   "ONLCR"},
    {OCRNL,   "OCRNL"},
    {ONOCR,   "ONOCR"},
    {ONLRET,  "ONLRET"},
    {OFDEL,   "OFDEL"},
#ifdef NLDLY
    {NLDLY,   "NLDLY"},
#endif
#ifdef CRDLY
{CRDLY,   "CRDLY"},
#endif
#ifdef TABDLY
    {TABDLY,  "TABDLY"},
#endif
#ifdef BSDLY
    {BSDLY,   "BSDLY"},
    #endif
#ifdef VTDLY
    {VTDLY,   "VTDLY"},
    #endif
};

static TermiosAttr cflags[]={
    {CLOCAL,  "CLOCAL"},
    {CREAD,   "CREAD"},
    {CS5,     "CS5"},
    {CS6,     "CS6"},
    {CS7,     "CS7"},
    {CS8,     "CS8"},
    {CSTOPB,  "CSTOPB"},
    {HUPCL,   "HUPCL"},
    {PARENB,  "PARENB"},
    {PARODD,  "PARODD"},
};

TermiosAttr cc[]={
    {VINTR,  "VINTR"},
    {VQUIT,  "VQUIT"},
    {VERASE, "VERASE"},
    {VKILL,  "VKILL"},
    {VEOF,   "VEOF"},
    {VTIME,  "VTIME"},
    {VMIN,   "VMIN"},
    {VSTART, "VSTART"},
    {VSTOP,  "VSTOP"},
    {VSUSP,  "VSUSP"},
    #ifdef VREPRINT
    {VREPRINT, "VREPRINT"},
    #endif
    #ifdef VWERASE
    {VWERASE, "VWERASE"},
#endif
#ifdef VLNEXT
    {VLNEXT, "VLNEXT"},
    #endif
};

void _free_TermiosCustom( void ){
    if( TermiosCustom != NULL )
        free( TermiosCustom );
    TermiosCustom= NULL;
    Termios_profiles_count= 0;
}

void _restore_termios( void ){
    if( termios_original_set ) log_error( EINVAL , TERMIOOPS , "restore_termios" , "Original termios attributes not set" );
    else
        for( int i= 0, attempts= 3, err_no ; ( err_no= tcsetattr( STDIN_FILENO , TCSANOW , &Termios_original ) ) && ( ++i <= attempts ) ; usleep( 10000 ) ){
            char err_msg[64];
            snprintf( err_msg , sizeof(err_msg) , "Error restoring original termios attributes (attempt %d)" , i );
            log_error( err_no , TERMIOOPS , "restore_termios" , ( i == attempts ) ? "Error restoring original termios attributes" : err_msg );
        }
}

void _termios_print( void ){
    puts("[c_lflag]");
    ITER_ATTR( lflags ) printf( "  %s: %s\n" , lflags[attr_idx].name , lflags[attr_idx].status ? "ON" : "OFF" );
    puts("[c_iflag]");
    ITER_ATTR( iflags ) printf( "  %s: %s\n" , iflags[attr_idx].name , iflags[attr_idx].status ? "ON" : "OFF" );
    puts("[c_oflag]");
    ITER_ATTR( oflags ) printf( "  %s: %s\n" , oflags[attr_idx].name , oflags[attr_idx].status ? "ON" : "OFF" );
    puts("[c_cflag]");
    ITER_ATTR( cflags ) printf( "  %s: %s\n" , cflags[attr_idx].name , cflags[attr_idx].status ? "ON" : "OFF" );
    puts("[c_cc]");
    ITER_ATTR( cc ) printf( "  %s: %d\n" , cc[attr_idx].name , cc[attr_idx].status );
}

void termiosexit( void ){
    _restore_termios( );
    _free_TermiosCustom( );
}

char termiosiszero( struct termios Termios ){
    struct termios Termios_zero;
    memset( &Termios_zero, 0 , sizeof( struct termios ) );
    return !memcmp( &Termios , &Termios_zero , sizeof( struct termios ));
}

struct termios termiosget( void ){
    struct  termios Termios;
    memset( &Termios , 0 , sizeof( struct termios ) );
    if( tcgetattr( STDIN_FILENO , &Termios) ){
        log_error( errno , TERMIOOPS , "termiosget" , "Error getting termios attributes" );
        memset( &Termios , 0 , sizeof( struct termios ) );
    }
    return Termios;
}

char termiosread( struct termios Termios ){
    ITER_ATTR( lflags ) lflags[attr_idx].status= !!(Termios.c_lflag & lflags[attr_idx].flag);
    ITER_ATTR( iflags ) iflags[attr_idx].status= !!(Termios.c_iflag & iflags[attr_idx].flag);
    ITER_ATTR( oflags ) oflags[attr_idx].status= !!(Termios.c_oflag & oflags[attr_idx].flag);
    ITER_ATTR( cflags ) cflags[attr_idx].status= !!(Termios.c_cflag & cflags[attr_idx].flag);
    ITER_ATTR( cc ){
        if( attr_idx >= NCCS ){
            log_error( EINVAL , TERMIOOPS , "termiosread" , "Invalid control character index" );
           return 1;
        }
        cc[attr_idx].status= Termios.c_cc[cc[attr_idx].flag];
    }
    return 0;
}

struct termios termioswrite( void ){
    struct termios Termios;
    memset( &Termios , 0 , sizeof( struct termios ) );
    ITER_ATTR( lflags ) if( lflags[attr_idx].status ) Termios.c_lflag|= lflags[attr_idx].flag; else Termios.c_lflag&= ~lflags[attr_idx].flag;
    ITER_ATTR( iflags ) if( iflags[attr_idx].status ) Termios.c_iflag|= iflags[attr_idx].flag; else Termios.c_iflag&= ~iflags[attr_idx].flag; 
    ITER_ATTR( oflags ) if( oflags[attr_idx].status ) Termios.c_oflag|= oflags[attr_idx].flag; else Termios.c_oflag&= ~oflags[attr_idx].flag; 
    ITER_ATTR( cflags ) if( cflags[attr_idx].status ) Termios.c_cflag|= cflags[attr_idx].flag; else Termios.c_cflag&= ~cflags[attr_idx].flag;  
    ITER_ATTR( cc ){
        if( attr_idx >= NCCS ){
            log_error( EINVAL , TERMIOOPS , "termioswrite" , "Invalid control character index" );
            return ( struct termios ){0};
        }
        Termios.c_cc[cc[attr_idx].flag]= cc[attr_idx].status ;
    }
    return Termios;
}

char termiosassign( char *label , char value){
    if( label == NULL ){
        log_error( EINVAL , TERMIOOPS , "termiosset" , "Label is NULL" );
        return 1;
    }
    ITER_ATTR( lflags ) if( !strcmp( label , lflags[attr_idx].name ) ){
        lflags[attr_idx].status= value;
        return 0;
    }
    ITER_ATTR( iflags ) if( !strcmp( label , iflags[attr_idx].name ) ){
        iflags[attr_idx].status= value;
        return 0;
    }
    ITER_ATTR( oflags ) if( !strcmp( label , oflags[attr_idx].name ) ){
        oflags[attr_idx].status= value;
        return 0;
    }
    ITER_ATTR( cflags ) if( !strcmp( label , cflags[attr_idx].name ) ){
        cflags[attr_idx].status= value;
        return 0;
    }
    ITER_ATTR( cc ) if( !strcmp( label , cc[attr_idx].name ) ){
        cc[attr_idx].status= value;
        return 0;
    }
    char err_msg[64]= "";
    snprintf( err_msg , sizeof(err_msg) , "Label %s not found" , label );
    log_error( EINVAL , TERMIOOPS , "termiosset" , err_msg );
    return 1;
}

char termiosregister( char *name , struct termios Termios ){
    if( !strlen( name ) || strlen( name ) >= TERMIOS_REG_MAX_NAME - 1 ){
        log_error( EINVAL , TERMIOOPS , "termiosregister" , "Invalid name" );
        return 1;
    }
    if( TermiosCustom == NULL ){
        if ( atexit( _free_TermiosCustom ) ){
            log_error( errno , TERMIOOPS , "termiosregister" , "Error setting exit function" );
            return 2;
        }
    } else for( int i= 0 ; i < Termios_profiles_count ; i++ ) if( !strcmp( name , TermiosCustom[i].name ) ) return 2;
    TermiosPreset *tmp= realloc( TermiosCustom , sizeof( TermiosPreset ) * ( Termios_profiles_count + 1) );
    if( tmp == NULL ){
        log_error( ENOMEM , TERMIOOPS , "termiosregister" , "Memory allocation error" );
        return 3;
    }
    TermiosCustom= tmp;
    strcpy( ( char *)TermiosCustom[Termios_profiles_count].name , name );
    TermiosCustom[Termios_profiles_count].termios_preset= Termios;
    Termios_profiles_count++;
    return 0;
}

char termiosload( char *name ){
    if( termios_original_set ){
        if( termiosiszero( Termios_original= termiosget( ) ) ){
            log_error( EINVAL , TERMIOOPS , "termiosload" , "Original termios attributes are zero" );
            return 1;
        }
        if( atexit( _restore_termios ) ){
            log_error( errno , TERMIOOPS , "termiosload" , "Error setting exit function" );
            return 2;
        }
        termios_original_set= !termios_original_set;
    }
    if( name == NULL || strlen( name ) >= TERMIOS_REG_MAX_NAME ){
        log_error( EINVAL , TERMIOOPS , "termiosload" , "Invalid name" );
        return 3;
    }
    int counter= 0;
    if( strlen( name ) ){
        if( TermiosCustom == NULL ){
            log_error( EINVAL , TERMIOOPS , "termiosload" , "No termios profiles registered" );
            return 4;
        }
        do{
            if( counter >= Termios_profiles_count ){
                log_error( EINVAL , TERMIOOPS , "termiosload" , "Profile not found" );
                return 5;
            }
        } while( strcmp( name , TermiosCustom[counter++].name ) );
    }
    if( tcsetattr( STDIN_FILENO , TCSANOW , counter ? &TermiosCustom[counter -1].termios_preset : &Termios_original ) ){
        log_error( errno , TERMIOOPS , "termiosload" , "Error setting termios attributes" );
        return 6;
    }
    previous_termios= current_termios;
    current_termios= counter;
    return 0;
}

void termiosswap( void ){
    if( termiosiszero( Termios_original ) ){
        log_error( EINVAL , TERMIOOPS , "termiosswap" , "Original termios not set" );
        return;
    }
    if( previous_termios == 0 ){
        if( tcsetattr( STDIN_FILENO ,TCSANOW , &Termios_original ) ){
            log_error( errno , TERMIOOPS , "termiosswap" , "Error setting original termios attributes" );
            return;
        }
    } else{
        if( TermiosCustom == NULL ){
            log_error( EINVAL , TERMIOOPS , "termiosswap" , "No termios profiles registered" );
            return;
        }
        if( tcsetattr( STDIN_FILENO ,TCSANOW , &TermiosCustom[previous_termios - 1].termios_preset ) ){
            log_error( errno , TERMIOOPS , "termiosswap" , "Error setting termios attributes" );
            return;
        }
    }
    previous_termios^= current_termios;
    current_termios^= previous_termios;
    previous_termios^= current_termios;
}

void termiosprtcurrent( void ){
    struct termios Termios;
    if( tcgetattr( STDIN_FILENO , &Termios ) ){
        log_error( errno , TERMIOOPS , "termiosptrcurrent" , "Error getting termios attributes" );
        return;
    }
    puts("\n======== Active terminal attributes ========\n");
    if( termiosread( Termios ) ) puts("Error reading termios attributes"); else _termios_print( );
    puts("\n============================================\n");
}

void termiosprt( struct termios Termios ){
    puts("\n========== Termios structure view ==========\n");
    if( termiosiszero( Termios ) ) puts( "No attributes set" );
    else if( termiosread( Termios ) ) puts("Error reading termios attributes");
    else _termios_print( );
    puts("\n============================================\n");
}

void termiosmatch( struct termios TermiosA , struct termios TermiosB ){
    TermiosA.c_lflag&= TermiosB.c_lflag;
    TermiosA.c_iflag&= TermiosB.c_iflag;
    TermiosA.c_oflag&= TermiosB.c_oflag;
    TermiosA.c_cflag&= TermiosB.c_cflag;
    for( int i= 0 ; i < NCCS ; i++ ) TermiosA.c_cc[i]= TermiosA.c_cc[i] == TermiosB.c_cc[i] ? TermiosA.c_cc[i] : 0;
    puts("\n======== Matched termios attributes ========\n");
    if( termiosiszero( TermiosA ) ) puts("No attributes matched");
    else if( termiosread( TermiosA )) puts("Error reading termios attributes");
    else{
        puts("[c_lflag]");
        ITER_ATTR( lflags ) if( lflags[attr_idx].status ) printf( "  %s\n" , lflags[attr_idx].name );
        puts("[c_iflag]");
        ITER_ATTR( iflags ) if( iflags[attr_idx].status ) printf( "  %s\n" , iflags[attr_idx].name );
        puts("[c_oflag]");
        ITER_ATTR( oflags ) if( oflags[attr_idx].status ) printf( "  %s\n" , oflags[attr_idx].name );
        puts("[c_cflag]");
        ITER_ATTR( cflags ) if( cflags[attr_idx].status ) printf( "  %s\n" , cflags[attr_idx].name );
        puts("[c_cc]");
        ITER_ATTR( cc ) if( cc[attr_idx].status ) printf( "  %s: %i\n" , cc[attr_idx].name , cc[attr_idx].status );
    }
    puts("\n============================================\n");
}