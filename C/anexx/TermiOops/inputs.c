#include "TermioOps.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>

char animation[]={ '#' , '=' , '-' , '+' };
#define SPEED 0.5

char kbhit( void ){
    fflush( stdout );
    char key= 0;
    static char termios_assigned= 0;
    if( !termios_assigned ){
        termios_assigned= 1;
        termiosload("");
        termiosassign("ICANON", 0);
        termiosassign("ECHO", 0); 
        termiosassign("ISIG", 0); 
        termiosassign("VMIN", 0); 
        termiosassign("VTIME", 0);
        termiosassign("IXON", 0);
        termiosassign("IXOFF", 0);
        struct termios Termios= termioswrite( );
        termiosregister("kbhit",Termios);
        termiosswap( );
    }
    termiosload("kbhit");
    read(STDIN_FILENO, &key, 1);
    termiosswap( );
    return key;
}


int main( void ){
    clock_t prev_time= clock( ), current_time;
    printf( "\033[?25l" );
    char animation_step= 0;
    char key= 0;
    while( key != 'Q' ){
        if( (((current_time= clock( )) - prev_time) / CLOCKS_PER_SEC) >= SPEED ){
            prev_time= current_time;
            printf( "\b%c" , animation[animation_step= (++animation_step) < sizeof( animation ) ? animation_step : 0] );
        }
        if( key= kbhit( ) ){
            printf( "\b%02X  " , key );
            while ( ((clock( ) - prev_time) / CLOCKS_PER_SEC) >= SPEED );
        }
    }
    printf( "\033[?25h" );
}