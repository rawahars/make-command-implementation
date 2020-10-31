#ifndef ASSIGNMENT3_TEXT_PARSER_H
#define ASSIGNMENT3_TEXT_PARSER_H

#include "error_handle.h"
#include "linked_list.h"

#define MAX_BUFFER_SIZE 4096

char* ReadLine(FILE* file);
list_node* ParseTargetString(char* line);
char* ParseCommandString(char* line);

#endif
