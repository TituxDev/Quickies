#include "bmp.h"
#include "transformations.h"

#define ESC 27

struct termios old_termios , new_termios;
void reset_termios( void ){
    tcsetattr( 0 , TCSANOW , &old_termios );
}
struct winsize ws;
void reset_screen( void ){
    printf( "\033[m\033[H\033[J" );
}

void *ptr[2]= { NULL , NULL };
void free_data( void ){
    uint8_t idx= 0;
    do{
        if( ptr[idx] ) free( ptr[idx] );
    } while( (idx= !idx) );
    
}

int main( int argc , char *argv[] ){
    if( argc < 2 ) return EXIT_FAILURE;
    file_t bmp_file= { .name= argv[1] , .out= argc > 2 ? argv[2] : "output.bmp" };
    
    atexit( free_data );
    if( !( bmp_file.file= fopen( bmp_file.name , "rb" ) )                                      ) return EXIT_FAILURE;
    if( !fread( &bmp_file.header , sizeof( header_t ) , 1 , bmp_file.file )                    ) return EXIT_FAILURE;
    if( bmp_file.header.signature != 0x4D42                                                    ) return EXIT_FAILURE;
    size_t extra_size= bmp_file.header.offset - sizeof( header_t );
    if( !( ptr[0]= bmp_file.extra= calloc( extra_size , 1 ) )                                  ) return EXIT_FAILURE;
    if( !fread( bmp_file.extra , extra_size , 1 , bmp_file.file )                              ) return EXIT_FAILURE;
    if( !( ptr[1]= bmp_file.bmp= calloc( bmp_file.header.image_size , 1 ) )                    ) return EXIT_FAILURE;
    if( !fread( bmp_file.bmp , bmp_file.header.image_size , 1 , bmp_file.file )                ) return EXIT_FAILURE;
    if( !( bmp_file.file= freopen( argc > 2 ? argv[2] : "output.bmp" , "wb" , bmp_file.file ) )) return EXIT_FAILURE;
    if( !fwrite( &bmp_file.header , sizeof( header_t ) , 1 , bmp_file.file )                   ) return EXIT_FAILURE;
    if( !fwrite( bmp_file.extra , extra_size , 1 , bmp_file.file )                             ) return EXIT_FAILURE;
    if( !fwrite( bmp_file.bmp , bmp_file.header.image_size , 1 , bmp_file.file )               ) return EXIT_FAILURE;
    rewind( bmp_file.file );

    atexit( reset_screen );
    ioctl( STDOUT_FILENO , TIOCGWINSZ , &ws );
    printf( "\033[H\033[J\033[36m\033[4mFILES :\033[m" );
    printf( "\n\t\033[35minput --> \033[m%s" , bmp_file.name );
    printf( "\n\t\033[35moutput -> \033[m%s" , bmp_file.out );
    printf( "\n\033[36m\033[4mHEADER :\033[m" );
    size_t header_field_legth= 0;
    for( size_t i= 0 ; i < TOTAL_HEADER_ATTRIBUTES ; i++ ) header_field_legth= header_field_legth > strlen( header_list[i].field ) ? header_field_legth : strlen( header_list[i].field );
    for( header_index i= 0 ; i < TOTAL_HEADER_ATTRIBUTES ; i++ ){
        printf( "\n\t\033[35m%s " , header_list[i].field );
        for( size_t j= strlen( header_list[i].field ) , ch= (i & 1) ? '-' : '=' ; j <= header_field_legth ; j++ ) putchar( ch );
        printf( "\033[m " );
        switch ( i ){
            case SIGNATURE:
                printf( "%c%c" , *((uint8_t *)&bmp_file.header + header_list[i].offset) , *((uint8_t *)&bmp_file.header + header_list[i].offset + 1) );
            break;
            case PLANES: case BITS_PER_PIXEL:
                printf( "%d" , *(uint16_t *)((uint8_t *)&bmp_file.header + header_list[i].offset) );
            break;
            default:
                printf( "%d" , *(uint32_t *)((uint8_t *)&bmp_file.header + header_list[i].offset) );
        }
    }
    printf( "\n\033[36m\033[4mEXTRA DATA :\n\033[m" );
    size_t row_legth= ws.ws_col - 8;
    row_legth/= 3;
    for( size_t i= 0 ; i < bmp_file.header.header_size - sizeof( header_t ) ; ){
        printf( "        " );
        for( size_t j= 0 ; j <= row_legth && i < (bmp_file.header.header_size - sizeof( header_t )) ; j++ ){
            if( bmp_file.extra[i] ) printf( "\033[35m");
            printf( "%02X \033[m" , bmp_file.extra[i++] );
        }
        putchar( '\n' );
    }
    printf( "\033[36m\033[4mOFFSET :\n\033[m" );
    for( size_t i= bmp_file.header.header_size - sizeof( header_t ) ; i < extra_size ; ){
        printf( "        " );
        for( size_t j= 0 ; j <= row_legth && i < extra_size ; j++ ){
            if( bmp_file.extra ) printf( "\033[35m" );
            printf( "%02X \033[m" , bmp_file.extra[i++] );
        }
        putchar( '\n' );
    }
    fflush( stdout );

    if( tcgetattr( 0 , &old_termios ) || atexit( reset_termios ) ) return EXIT_FAILURE;
    new_termios= old_termios;
    new_termios.c_lflag&= ~( ECHO | ICANON | ISIG );
    new_termios.c_iflag&= ~( IXON | IXOFF );
    new_termios.c_cc[VMIN]= 0;
    new_termios.c_cc[VTIME]= 0;
    tcsetattr( 0 , TCSANOW , &new_termios );

    fflush( stdin );
    for( uint8_t key[6]= { 0 } , option= 1 ; option ; ){
        if( read( STDIN_FILENO , key , sizeof( key ) ) ){
            switch( key[0] ){
                case '\n':
                    goto EXECUTE;
                case ESC:
                    switch( key[1] ){
                        case '[':
                            switch( key[2] ){
                                case 'A':
                                    printf( "\r%i" , ++option);
                                    fflush( stdout );
                                break;
                                case 'B':
                                    printf( "\r%i " , option-= !!(option - 1) );
                                    fflush( stdout );
                                break;
                                case 'C':
                                break;
                                case 'D':
                                break;
                                default:
                                    option= 0;
                            }
                        break;
                        default:
                            option= 0;
                    }
                break;
                default:
                    option= 0;
            }
            memset( key , 0 , sizeof( key ) );
        } EXECUTE:

        if( key[0] == '\n' ) {
            key[0]= 0;
            apply( option - 1 , &bmp_file);
            fseek( bmp_file.file , bmp_file.header.offset , SEEK_SET );
            fwrite( bmp_file.bmp , bmp_file.header.image_size , 1 , bmp_file.file );
            fflush( bmp_file.file );
            rewind( bmp_file.file );
            option = 1;
        }
    }

    fclose( bmp_file.file );

    return EXIT_SUCCESS;
}