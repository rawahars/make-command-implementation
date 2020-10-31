#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "text_parser.h"

int filter(char* line);
int isFirstCharacterIsLetterOrTab(char* line);
int hasOnlySpaces(char* line);
char* extractLineFromFile(FILE* file);
char* reallocateString(char* buffer, int len);

char* ReadLine(FILE* file){
    char* line = extractLineFromFile(file);
    if(!filter(line)){
        return NULL;
    }
    return line;
}

list_node* ParseTargetString(char* line){
    list_node* target = CreateLinkedList();
    int len = strlen(line);
    int index = 0;

    // Extract the target name from the target line
    char* currStr = malloc(sizeof(char)*MAX_BUFFER_SIZE);
    for(; index < len; index++){
        if(line[index] == ':'){
            currStr[index] = '\0';
            break;
        }
        currStr[index] = line[index];
    }
    if(index == len){// No : found in the target string
        TargetParsingError();
    }
    char* str = reallocateString(currStr, index);
    AddNode(target, str);

    // Extract dependencies of the target
    int initialIndex = index+1;
    currStr = malloc(sizeof(char)*MAX_BUFFER_SIZE);
    for(index = index+1; index < len; index++){
        if(line[index] == ' ' || line[index] == '\t'){
            if((index - initialIndex) == 0){
                initialIndex++;
            } else {
                currStr[(index - initialIndex)] = '\0';
                str = reallocateString(currStr, (index - initialIndex));
                AddNode(target, str);
                currStr = malloc(sizeof(char)*MAX_BUFFER_SIZE);
                initialIndex = index + 1;
            }
        } else {
            currStr[(index - initialIndex)] = line[index];
        }
    }

    if((index - initialIndex) != 0){
        currStr[len - initialIndex] = '\0';
        str = reallocateString(currStr, (index - initialIndex));
        AddNode(target, str);
    }
    return target;
}

char* ParseCommandString(char* line){
    int len = strlen(line);
    char* newCommand = malloc(sizeof(char)*len);
    for(int i = 1; i < len; i++)
        newCommand[i-1] = line[i];
    newCommand[len-1] = '\0';
    free(line);
    return newCommand;
}

int filter(char* line){
    if(strlen(line) == 0 || hasOnlySpaces(line) || line[0] == '#' || isFirstCharacterIsLetterOrTab(line)){
        return 0;
    }
    return 1;
}

int isFirstCharacterIsLetterOrTab(char* line){
    int len = strlen(line);
    if(isalnum(line[0]) || (len > 1 && line[0] == '\t' && isalnum(line[1])))
        return 1;
    else
        return 0;
}

int hasOnlySpaces(char* line){
    int len = strlen(line);
    for(int i=0; i < len; i++){
        if(line[i] != ' ') return 0;
    }
    return 1;
}

char* extractLineFromFile(FILE* file){
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

    char *final_str = reallocateString(buffer, len);
    return final_str;
}

char* reallocateString(char* buffer, int len){
    char* final_str = malloc(sizeof(char)*len);
    strcpy(final_str, buffer);
    free(buffer);
    return final_str;
}