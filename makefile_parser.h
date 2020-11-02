#ifndef ASSIGNMENT3_MAKEFILE_PARSER_H
#define ASSIGNMENT3_MAKEFILE_PARSER_H

#include "text_parser.h"
#include "graph.h"
#include "linked_list.h"
#include "error_handle.h"

typedef struct{
    char* target_name;
    // isInitialized is set to 1 only after the rule's dependencies and commands are set
    int isInitialized;
    // Each dependency would be a rule itself
    list_node* dependencies;
    // List of commands for this rule
    list_node* commands;
} rule;

typedef struct{
    char* command_name;
    list_node* command_args;
} command;

vertex* FindRuleVertex(list_node* list, char* current_rule_name);
list_node* ParseMakefile(FILE* file);

#endif
