#ifndef ASSIGNMENT3_TEXT_PARSER_H
#define ASSIGNMENT3_TEXT_PARSER_H

#include <stdio.h>
#include "error_handle.h"
#include "linked_list.h"

#define MAX_BUFFER_SIZE 4096

char *ReadLine(FILE *file, int index);

list_node *ParseTargetString(char *line, int line_index);

list_node *ParseCommandString(char *line);

#endif
