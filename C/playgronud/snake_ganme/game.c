#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>

#define INITIAL_DIRECTION UP
#define INITIAL_SIZE 11
#define INITIAL_SPEED 0.5

enum simbols_list{
    BLANK ,
    SPACE ,
    BLOCK ,
    TOP ,
    BOTTOM ,

    TOTAL_SIMBOLS
};
char *simbol[TOTAL_SIMBOLS]={
    [BLANK]= "" ,
    [SPACE]= " " ,
    [BLOCK]= "█" ,
    [TOP]= "▀" ,
    [BOTTOM]= "▄"
};

#define ESC 27
enum direction{ DEFAULT , LEFT , RIGHT , DOWN , UP };
#define gotoxy( x , y , print ) printf( "\033[%i;%iH%s",  y , x , print );

typedef struct {
    int x;
    int y;
} body_t;
body_t *snake= NULL , tail={ 0 , 0 };
struct winsize ws;
clock_t current_time, prev_time;
struct termios old_term , new_term;

char head_direction= INITIAL_DIRECTION <= DEFAULT || INITIAL_DIRECTION > 4 ? RIGHT : INITIAL_DIRECTION;
char tail_direction= INITIAL_DIRECTION <= DEFAULT || INITIAL_DIRECTION > 4 ? RIGHT : INITIAL_DIRECTION;;
size_t snake_size= INITIAL_SIZE < 3 ? 3 : INITIAL_SIZE;
double delta_time , speed= INITIAL_SPEED < 0.1 ? 0.1 : INITIAL_SPEED;

char again= 1;
char key[4]= "";

void reset_attributes( void );

int main( void ){
    tcgetattr( STDIN_FILENO , &old_term );
    new_term= old_term;
    new_term.c_lflag&= ~( ICANON | ECHO | ISIG );
    new_term.c_iflag&= ~( IXON | IXOFF );
    new_term.c_cc[VMIN]= 0;
    new_term.c_cc[VTIME]= 0;
    tcsetattr( STDIN_FILENO , TCSANOW , &new_term );
    printf("\033[2J\033[?25l");
    atexit( reset_attributes );

    ioctl( STDOUT_FILENO , TIOCGWINSZ , &ws );
    char grid[ws.ws_row][ws.ws_col];
    memset( grid , '\0' , ws.ws_row * ws.ws_col );

    snake= calloc( sizeof( body_t ) , snake_size );
    for( size_t i= 0 ; i < snake_size ; i++ ){
        switch ( head_direction ){
            case LEFT:
                snake[i].x= ws.ws_col - snake_size + 1 + i;
                snake[i].y= ws.ws_row;
                grid[snake[i].y / 2 + ( snake[i].y  & 1 )][snake[i].x]= snake[i].y & 1 ? TOP : BOTTOM;
            break;
            case RIGHT:
                snake[i].x= snake_size - i;
                snake[i].y= ws.ws_row;
                grid[snake[i].y / 2 + ( snake[i].y  & 1 )][snake[i].x]= snake[i].y & 1 ? TOP : BOTTOM;
            break;
            case DOWN:
                snake[i].x= ws.ws_col / 2;
                snake[i].y= snake_size - i;
                grid[snake[i].y / 2 + ( snake[i].y  & 1 )][snake[i].x]= snake[i].y  & 1 ? BLOCK : TOP;
            break;
            case UP:
                snake[i].x= ws.ws_col / 2;
                snake[i].y= ws.ws_row * 2 - snake_size + 1 + i;
                grid[snake[i].y / 2 + ( snake[i].y  & 1 )][snake[i].x]= snake[i].y  & 1 ? BOTTOM : BLOCK;
            }
            gotoxy( snake[i].x , snake[i].y / 2 + ( snake[i].y  & 1 ) , simbol[grid[snake[i].y / 2 + ( snake[i].y  & 1 )][snake[i].x]] );
            fflush( stdout );
    }

    prev_time= clock( );
    while( again ){
        current_time= clock();
        delta_time= (double)(current_time - prev_time) / CLOCKS_PER_SEC;

        if( delta_time >= speed ) {
            prev_time= current_time;

            memcpy( &tail , &snake[snake_size - 1] , sizeof( body_t ) );
            memmove( &snake[1] , &snake[0] , ( snake_size - 1 ) * sizeof( body_t ) );
            switch( head_direction ){
                case LEFT: snake[0].x--; break;
                case RIGHT: snake[0].x++; break;
                case DOWN: snake[0].y++; break;
                case UP: snake[0].y--;
            }

            if( snake[0].y & 1 ) grid[snake[0].y / 2 + ( snake[0].y & 1 )][snake[0].x]= grid[snake[0].y / 2 + ( snake[0].y & 1 )][snake[0].x] == BOTTOM ? BLOCK : TOP;
            else grid[snake[0].y / 2 + ( snake[0].y & 1 )][snake[0].x]= grid[snake[0].y / 2 + ( snake[0].y & 1 )][snake[0].x] == TOP ? BLOCK : BOTTOM;
            if( tail.y & 1 ) grid[tail.y / 2 + ( tail.y & 1 )][tail.x]= grid[tail.y / 2 + ( tail.y & 1 )][tail.x] == BLOCK ? BOTTOM : SPACE;
            else grid[tail.y / 2 + ( tail.y & 1 )][tail.x]= grid[tail.y / 2 + ( tail.y & 1 )][tail.x] == BLOCK ? TOP : SPACE;

            gotoxy( tail.x , tail.y / 2 + ( tail.y & 1 ) , simbol[grid[tail.y / 2 + ( tail.y & 1 )][tail.x]] );
            gotoxy( snake[0].x , snake[0].y / 2 + ( snake[0].y & 1 ) , simbol[grid[snake[0].y / 2 + ( snake[0].y & 1 )][snake[0].x]] );

            if( !( snake[0].y && snake[0].x ) || snake[0].x >= ws.ws_col || snake[0].y >= ws.ws_row * 2 ) again= 0;
            for( size_t i= 1 ; i < snake_size ; i++ ) if( snake[0].x == snake[i].x && snake[0].y == snake[i].y ) again= 0;

            fflush( stdout );
        }

        char i= 0;
        while( read( STDIN_FILENO , &key[i++] , 1 ) );
        switch( key[0] ){
            case '+': speed-= 0.1; if( speed < 0.1 ) speed= 0.1; break;
            case '-': speed+= 0.1; break;
            case ESC: switch ( key[1] ){
                case '[': switch( key[2] ){
                    case 'A': if( head_direction ^ DOWN ) head_direction= UP; break;
                    case 'B': if( head_direction ^ UP ) head_direction= DOWN; break;
                    case 'C': if( head_direction ^ LEFT ) head_direction= RIGHT; break;
                    case 'D': if( head_direction ^ RIGHT ) head_direction= LEFT; break;
                } break;
                default: again= 0;
            }
        }
        memset( key , '\0' , sizeof( key ) );
    }

    return 0;
}

void reset_attributes( void ){
    tcsetattr( STDIN_FILENO , TCSANOW , &old_term );
    free( snake );
    printf("\033[?25h");
    fflush( stdout );
}