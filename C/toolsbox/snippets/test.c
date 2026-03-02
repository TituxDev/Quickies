#include "snippets.h"

const uchar functionlist[_FUNCTION_LIST][20]={ 
    [_ISDIGIT]=     "ISDIGIT" ,
    [_ISALPHAU]=    "ISALPHAU" ,
    [_ISALPHAL]=    "ISALPHAL" ,
    [_ISCONTROL]=   "ISCONTROL" ,
    [_ISBLANK]=     "ISBLANK" ,
    [_TOUINT]=      "TOUINT" ,
    [_UPPER]=       "UPPER" ,
    [_LOWER]=       "LOWER" ,
    [_ISNOTPAR]=    "ISNOTPAR" ,
    [_BIGGER]=      "BIGGER" ,
    [_SMALLER]=     "SMALLER" ,
    [_ABS]=         "ABS" ,
    [_MAXVALUE]=    "MAXVALUE" ,
    [_ROUNDUP]=     "ROUNDUP" ,
    [_BOOLEAN]=     "BOOLEAN" ,
    [_SWITCH]=      "SWITCH" ,
    [_SWAP]=        "SWAP" ,
    [_COUNT]=       "COUNT" ,
    [_ERASE]=       "ERASE" ,
    [_COPY]=        "COPY" ,
    [_OFFSET]=      "OFFSET" ,
    [_BIT]=         "BIT" ,
    [_ISBITUP]=     "ISBITUP" ,
    [_ISMASKON]=    "ISMASKON" ,
    [_UBITS]=       "UBITS" ,
    [_DBITS]=       "DBITS" ,
    [_SBITS]=       "SBITS"
} ;
const char answer[2][4]={
    [0]=    "NO" ,
    [1]=    "YES"
} ;

uchar copyargv[COUNT( *functionlist )] ;
uchar functionselect ;
long aux ;
typedef struct{
    char a ;
    short b ;
    int c ;
    long d ;
    long e[2] ;
    char f ;
} auxstruct ;
void *structpointer[6] ;

#include <stdio.h>
const char strlegth= SMALLER( COUNT( *functionlist ) , COUNT( copyargv ) ) ;

int main ( int argc , char **argv){
    if ( argc < 2 ) return 255 ;
    ERASE( copyargv ) ;
    COPY( argv[1] , copyargv ) copyargv[copy_i]= UPPER( copyargv[copy_i] ) ;
    for ( functionselect = 0 ; functionselect < _FUNCTION_LIST && !comparr( copyargv , ( uchar * )functionlist[functionselect] , strlegth ); functionselect++ ) ;
    printf( "\n" ) ;
    switch ( functionselect ){

        case _ISDIGIT : 

            if( ( aux= argc < 3 ) ) break ;
            printf( "Is ASCII( %i ) a numeric character?: %s" , argv[2][0] , answer[ISDIGIT( argv[2][0] )] ) ;
            break ;

        case _ISALPHAU : 

            if( ( aux= argc < 3 ) ) break ;
            printf( "Is ASCII( %i ) an upper case character?: %s" , argv[2][0] , answer[ISALPHAU( argv[2][0] )] ) ;
            break;

        case _ISALPHAL :

            if( ( aux= argc < 3 ) ) break ;
            printf( "Is ASCII( %i ) a lower case character?: %s" , argv[2][0] , answer[ISALPHAL( argv[2][0] )] ) ;
            break ;

        case _ISCONTROL : 

            if ( ( aux= argc < 3 ) ) break ;
            aux= strtouint( (uchar *)argv[2] ) ;
            printf( "Is a control character?: %s" , answer[ISCONTROL( aux )] ) ;
            aux= 0 ;
            break ;

        case _ISBLANK :

            if( ( aux= argc < 3 ) ) break ;
            aux= strtouint( (uchar *)argv[2] ) ;
            printf( "Is no printable character?: %s" , answer[ISBLANK( aux )] ) ;
            aux= 0 ;
            break ;

        case _TOUINT :

            if( ( aux= argc < 3 ) ) break ; 
            printf( "Is ASCII( %i ) a numeric character?: %s\nConvert to decimal value: %i" , argv[2][0] , answer[ISDIGIT( argv[2][0] )] , TOUINT( argv[2][0] ) ) ;
            break ;

        case _UPPER :        
        
            if( ( aux= argc < 3 ) ) break ;
            printf( "Is ASCII( %i ) a letter?: %s\nUpper case: %c" , argv[2][0] , answer[ISALPHAL( argv[2][0] ) || ISALPHAU( argv[2][0] )] , UPPER( argv[2][0] ) ) ;
            break ;

        case _LOWER :
        
            if( ( aux= argc < 3 ) ) break ;
            printf( "Is ASCII( %i ) a letter?: %s\nLower case: %c" , argv[2][0] , answer[ISALPHAL( argv[2][0] ) || ISALPHAU( argv[2][0] )] , LOWER( argv[2][0] ) ) ;
            return aux ;

        case _ISNOTPAR :    
        
            if( ( aux= argc < 3 ) ) break ;
            for( uchar i= argv[2][0] == '-' ; argv[2][i] ; i++ ) if( !ISDIGIT( argv[2][i] ) ) return 251 ;
            printf( "Is even number?: %s" , answer[ISNOTPAR( strtoint( (uchar *)argv[2] ) )] ) ;
            break ;

        case _BIGGER :      
            
            if( ( aux= argc < 4 ) ) break ;
            printf( "ASCII_A: %i\tASCII_B: %i\nBigger: %i", argv[2][0] , argv[3][0] , BIGGER( argv[2][0] , argv[3][0] ) ) ;
            break ;

        case _SMALLER :     
        
            if( ( aux= argc < 4 ) ) break ;
            printf( "ASCII_A: %i\tASCII_B: %i\nSmaller: %i", argv[2][0] , argv[3][0] , SMALLER( argv[2][0] , argv[3][0] ) ) ;
            break ;

        case _ABS :         
        
            if( ( aux= argc < 3 ) ) break ;
            for( uint i= argv[2][0] == '-' ; argv[2][i] ; i++ ) if( !ISDIGIT( argv[ 2 ][ i ] ) ) return 251 ;
            printf( "Absolute value: %i" , ABS( strtoint( (uchar *)&argv[ 2 ][ 0 ] ) ) ) ;
            break ;

        case _MAXVALUE :    
        
            aux= 0 ;
            printf( "\tSIGNED\t\t\tUNSIGNED" ) ;
            printf( "\nchar:\t%lu\t\t\t%lu" , MAXVALUE( char ) , MAXVALUE( unsigned char ) ) ;
            printf( "\nshort:\t%lu\t\t\t%lu" , MAXVALUE( short ) , MAXVALUE( unsigned short ) ) ;
            printf( "\nint:\t%lu\t\t%lu" , MAXVALUE( int ) , MAXVALUE( unsigned int ) ) ;
            printf( "\nfloat:\t%lu\t\tX" , MAXVALUE( float ) ) ;
            printf( "\nlong:\t%lu\t%lu" , MAXVALUE( long ) , MAXVALUE( unsigned long ) ) ;
            printf( "\ndouble:\t%lu\tX" , MAXVALUE( double ) ) ;
            break ;

        case _ROUNDUP :     
        
            if( ( aux= argc < 4 ) ) break ;
            printf( "Next number that is multiple of base from factor: %i" , ROUNDUP( strtoint( (uchar *)argv[2] ) , strtoint( (uchar *)argv[3] ) ) ) ;
            break ;

        case _BOOLEAN :     
        
            if( ( aux= argc < 3 )) break ;
            for( uint i= 0 ; argv[2][i] ; i++ ) if( !ISDIGIT( argv[2][i] ) ) return 251 ;
            printf("Boolean value: %i" , BOOLEAN( strtouint( (uchar *)argv[2] ) ) ) ;
            break ;
        
        case _SWITCH :      
        
            if( ( aux= argc < 3 ) ) break ;
            aux= TOUINT( argv[2][0] ) ;
            printf( "Switched value: %li" , SWITCH( aux ) ) ;
            aux= 0 ;
            break ;

        case _SWAP :         
        
            if( ( aux= argc < 3 ) ) break ;
            copyargv[0]= strtouint( (uchar *)argv[2] ) ;
            copyargv[1]= strtouint( (uchar *)argv[3] ) ;
            printf( "A: %u\tB: %u\n", copyargv[0] , copyargv[1] ) ;
            SWAP( uchar , copyargv[0] , copyargv[1] ) ;
            printf( "A: %u\tB: %u", copyargv[0] , copyargv[1] ) ;
            break ;

        case _COUNT :        
        
            aux= 0 ;
            printf( "Macros listed: %lu\nMaximum characters per label: %lu" , COUNT( functionlist ) , COUNT( *functionlist ) ) ;
            break ;

        case _ERASE :
        
            if( ( aux= argc < 3 ) ) break ;
            COPY( argv[2] , copyargv ) ;
            printf( "\nVaules in array: " ) ;
            for( uchar i= 0, STOP= ( uchar )COUNT( copyargv ) ; i < STOP ; printf( "%i," , copyargv[i++] ) ) ;
            printf( "\nEraising data from array..." ) ;
            ERASE( copyargv ) ;
            printf( "\nValues in array: " ) ;
            for( uchar i= 0, STOP= ( uchar )COUNT( copyargv ) ; i < STOP ; printf( "%i," , copyargv[i++] ) ) ;
            break ;

        case _COPY :
        
            if( ( aux= argc < 3 ) ) break ;
            ERASE( copyargv ) ;
            printf( "\nValues in array: " ) ;
            for( uchar i= 0, STOP= ( uchar )COUNT( copyargv ) ; i < STOP ; printf( "%i," , copyargv[i++] ) ) ;
            printf( "Copying from input to an array..." ) ;
            COPY( argv[2] , copyargv ) ;
            printf( "\nVaules in array: " ) ;
            for( uchar i= 0, STOP= ( uchar )COUNT( copyargv ) ; i < STOP ; printf( "%i," , copyargv[i++] ) ) ;
            break ;

        case _OFFSET :       
        
            aux= 0 ; //Pendiente de evaluar si es posible contar lor miembros de una estructura
            structpointer[0]= OFFSET( auxstruct , a ) ;
            structpointer[1]= OFFSET( auxstruct , b ) ;
            structpointer[2]= OFFSET( auxstruct , c ) ;
            structpointer[3]= OFFSET( auxstruct , d ) ;
            structpointer[4]= OFFSET( auxstruct , e ) ;
            structpointer[5]= OFFSET( auxstruct , f ) ;
            for( uint i= 0, STOP= COUNT( structpointer ) ; i < STOP ; printf( "\n%p", structpointer[i++] ) ) ;
            break ;


        case _BIT :          
        
            if( ( aux= argc < 3 ) ) break ;
            for( uint j= 2 ; j < argc ; j++ ) for( uint i= 0 ; argv[j][i] ; i++ ) if( ISDIGIT( argv[j][i] ) )  copyargv[j - 2]= strtouint( (uchar *)argv[j] ); else return 251 ;
            for( uint i= 0 ; i < argc - 2 ; copyargv[0]|= BIT( copyargv[i++] ) ) printf( "\nDecimal value : %i" , BIT( copyargv[i] ) ) ;
            printf( "\n\nBITMASK value: %i" , copyargv[0] ) ;
            break ;

        case _ISBITUP :
            
            if( ( aux= argc < 4 ) ) break ;
            for( uint j= 2 ; j < argc ; j++ ) for( uint i= 0 ; argv[j][i] ; i++ ) if( ISDIGIT( argv[j][i] ) )  copyargv[j - 2]= strtouint( (uchar *)argv[j] ); else return 251 ;
            printf( "BITMASK_A:\t" ) ;
            for( uint i= strtoint( (uchar *)argv[2] ) ; i ; i= i >> 1 ) printf( "%i" , ISBITUP( i , 1 ) ) ;
            printf( "\nBITMASK_B:\t" ) ;
            for( uint i= strtoint( (uchar *)argv[3] ) ; i ; i= i >> 1 ) printf( "%i" , ISBITUP( i , 1 ) ) ;
            printf( "\nBIT_AND:\t" ) ;
            for( uint i= ISBITUP( strtouint( (uchar *)argv[2] ) , strtouint( (uchar *)argv[3] ) ) ; i ; i= i >> 1 ) printf( "%i" , ISBITUP( i , 1 ) ) ;
            printf( "\nDecimal value:\t%i" , ISBITUP( strtouint( (uchar *)argv[2] ) , strtouint( (uchar *)argv[3] ) ) ) ;
            printf( "\nIs there at least one up bit coincidence? %s" , answer[BOOLEAN( ISBITUP( strtouint( (uchar *)argv[2] ) , strtouint( (uchar *)argv[3] ) ) )] ) ;
            break ;

        case _ISMASKON :    
        
            if( ( aux= argc < 4 ) ) break ;
            for( uint j= 2 ; j < argc ; j++ ) for( uint i= 0 ; argv[j][i] ; i++ ) if( ISDIGIT( argv[j][i] ) )  copyargv[j - 2]= strtouint( (uchar *)argv[j] ); else return 251 ;
            printf( "BITMASK_A:\t" ) ;
            for( uint i= strtoint( (uchar *)argv[2] ) ; i ; i= i >> 1 ) printf( "%i" , ISBITUP( i , 1 ) ) ;
            printf( "\nBITMASK_B:\t" ) ;
            for( uint i= strtoint( (uchar *)argv[3] ) ; i ; i= i >> 1 ) printf( "%i" , ISBITUP( i , 1 ) ) ;
            printf( "\nAre all the bits up in B  also up in A? %s" , answer[ISMASKON( strtouint( (uchar *)argv[2] ) , strtouint( (uchar *)argv[3] ) )] ) ;
            break ;

        case _UBITS :       
        
            if( ( aux= argc < 4 ) ) break ;
            for( uint j= 2 ; j < argc ; j++ ) for( uint i= 0 ; argv[j][i] ; i++ ) if( ISDIGIT( argv[j][i] ) )  copyargv[j - 2]= strtouint( (uchar *)argv[j] ) ; else return 251 ;
            printf( "BITMASK:\t" ) ;
            for( uint i= strtoint( (uchar *)argv[2] ) ; i ; i= i >> 1 ) printf( "%i" , ISBITUP( i , 1 ) ) ;
            printf( "\nUPMASK:\t\t" ) ;
            for( uint i= strtoint( (uchar *)argv[3] ) ; i ; i= i >> 1 ) printf( "%i" , ISBITUP( i , 1 ) ) ;
            printf( "\nFINAL MASK:\t" ) ;
            aux= strtouint( (uchar *)argv[2] ) ;
            for( UBITS( aux , strtouint( (uchar *)argv[3] ) ) ; aux ; aux= aux >> 1 ) printf( "%li", ISBITUP( aux , 1 ) ) ;
            aux= strtouint( (uchar *)argv[2] ) ;
            printf( "\nDecimal value:\t%li" , UBITS( aux , strtouint( (uchar *)argv[3] ) ) ) ;
            aux= 0 ;
            break ;

        case _DBITS :       
        
            if( ( aux= argc < 4 ) ) break ;
            for( uint j= 2 ; j < argc ; j++ ) for( uint i= 0 ; argv[j][i] ; i++ ) if( ISDIGIT( argv[j][i] ) )  copyargv[j - 2]= strtouint( (uchar *)argv[j] ) ; else return 251 ;
            printf( "BITMASK:\t" ) ;
            for( uint i= strtoint( (uchar *)argv[2] ) ; i ; i= i >> 1 ) printf( "%i" , ISBITUP( i , 1 ) ) ;
            printf( "\nDOWNMASK:\t" ) ;
            for( uint i= strtoint( (uchar *)argv[3] ) ; i ; i= (unsigned long)i >> 1 ) printf( "%i" , ISBITUP( i , 1 ) ) ;
            printf( "\nFINAL MASK:\t" ) ;
            aux= strtouint( (uchar *)argv[2] ) ;
            for( DBITS( aux , strtouint( (uchar *)argv[3] ) ) ; aux ; aux= aux >> 1 ) printf( "%li" , ISBITUP( aux , 1 ) ) ;
            aux= strtouint( (uchar *)argv[2] ) ;
            printf( "\nDecimal value:\t%li" , DBITS( aux , strtouint( (uchar *)argv[3] ) ) ) ;
            aux= 0 ;
            break ;

        case _SBITS :       
        
            if( ( aux= argc < 4 ) ) break ;
            for( uint j= 2 ; j < argc ; j++ ) for( uint i= 0 ; argv[j][i] ; i++ ) if( ISDIGIT( argv[j][i] ) )  copyargv[j - 2]= strtouint( (uchar *)argv[j] ) ; else return 251 ;
            printf( "BITMASK:\t" ) ;
            for( uint i= strtoint( (uchar *)argv[2] ) ; i ; i= i >> 1 ) printf( "%i" , ISBITUP( i , 1 ) ) ;
            printf( "\nFLIPMASK:\t" ) ;
            for( uint i= strtoint( (uchar *)argv[3] ) ; i ; i= (unsigned long)i >> 1 ) printf( "%i" , ISBITUP( i , 1 ) ) ;
            printf( "\nFINAL MASK:\t" ) ;
            aux= strtouint( (uchar *)argv[2] ) ;
            for( SBITS( aux , strtouint( (uchar *)argv[3] ) ) ; aux ; aux= aux >> 1 ) printf( "%li", ISBITUP( aux , 1 ) ) ;
            aux= strtouint( (uchar *)argv[2] ) ;
            printf( "\nDecimal value:\t%li" , SBITS( aux , strtouint( (uchar *)argv[3] ) ) ) ;
            aux= 0 ;
            break ;

        case _FUNCTION_LIST :

            return 254 ;
        
        default :            
        
            return 253 ;
    }
    printf( "\n\n" ) ;
    return aux * 252 ;
}