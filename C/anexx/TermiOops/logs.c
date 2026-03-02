#include "logs.h"
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#ifndef ERROR_LOG
#define ERROR_LOG 1
#else
#undef ERROR_LOG
#define ERROR_LOG 0
#endif

void log_error( const int error_no, const char *library, const char *function, const char *message){
    if( ERROR_LOG ) return;
    time_t raw_time;
    time( &raw_time );
    struct tm *timeinfo= localtime( &raw_time );
    char time_str[20];
    strftime( time_str , sizeof( time_str ) , "%F %T" , timeinfo );
    char path[64]= "";
    snprintf( path , sizeof(path) , "%s_error.log", library);
    FILE *error_log= NULL;
    error_log= fopen( path , "a" );
    fprintf( error_log , "(%i) %s | %s | lib[%s] func:[%s] **%s**\n", error_no , strerror( error_no) , time_str , library , function , message);
    fclose( error_log );
}