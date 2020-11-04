/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#ifndef ASSIGNMENT3_MAKEFILE_PARSER_H
#define ASSIGNMENT3_MAKEFILE_PARSER_H

#include "text_parser.h"
#include "graph.h"
#include "linked_list.h"
#include "error_handle.h"

typedef struct {
    // name of the target rule
    char *target_name;
    // isInitialized is set to 1 only after the rule's dependencies and commands are set
    // Essentially it is 0 for files
    int isInitialized;
    // Each dependency would be a rule itself
    list_node *dependencies;
    // List of commands for this rule
    list_node *commands;
    // Index at which this target appears in the makefile
    int target_index;
    // Original contents of the line
    char *target_str;
} rule;

typedef struct {
    // name of the command
    char *command_name;
    // list of all the arguments of the command
    list_node *command_args;
    // index at which this command was present in the makefile
    int cmd_index;
    // complete string which was present in the makefile
    char *cmd_string;
} command;

list_node *ParseMakefile(FILE *file);

vertex *FindRuleVertex(list_node *list, char *current_rule_name);

void PrintExecutionGraph(list_node *current_node);

int DetectCycleInGraph(list_node *start);

#endif
