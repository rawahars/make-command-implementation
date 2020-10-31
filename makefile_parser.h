#ifndef ASSIGNMENT3_MAKEFILE_PARSER_H
#define ASSIGNMENT3_MAKEFILE_PARSER_H

#include "linked_list.h"

typedef struct target{
    char* dependency_names;
    list_node* dependencies;
    list_node* commands;
};

typedef struct dependency {
    char* name;
};

typedef struct command{
    char* command_spec;
};

#endif
