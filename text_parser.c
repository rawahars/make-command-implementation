#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text_parser.h"

char* ReadLine(FILE* file){
    char* buffer = malloc(sizeof(char)*MAX_BUFFER_SIZE);
    int len = 0;
    char ch;

    while(1){
        ch = fgetc(file);

        if(len >= MAX_BUFFER_SIZE){ // During buffer overflow exit with error
            free(buffer);
            BufferOverflowError();
        } else if(ch == '\0'){ // If a null byte is found in line then exit with error
            NullByteInLineError();
        } else if(ch == EOF || ch == '\n'){
            buffer[len++] = '\0';
            break;
        } else {
            buffer[len++] = ch;
        }
    }

    char* final_str = malloc(sizeof(char)*len);
    strcpy(final_str, buffer);
    free(buffer);

    return final_str;
}

