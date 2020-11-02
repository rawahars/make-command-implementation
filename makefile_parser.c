#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "makefile_parser.h"

rule *initializeRule(char *target_name, list_node *dependencies, int isInitialized);

void saveRule(list_node *list, rule *current_rule);

void setDependenciesOfRule(list_node *list, vertex *vert, list_node *dependencies);

command *parseCommand(list_node *list);

list_node *ParseMakefile(FILE *file) {
    list_node *list_of_vertices = CreateLinkedList();
    char *line;
    list_node *parsed_string;
    int isTargetString = 0;
    rule *current_rule = NULL;

    while (feof(file) == 0) {
        line = ReadLine(file);

        if (line == NULL) continue;
        else if (line[0] == '\t') {
            parsed_string = ParseCommandString(line);
            if (current_rule == NULL) {
                InvalidCommandInMakefileError();
            }
            isTargetString = 0;
        } else {
            parsed_string = ParseTargetString(line);
            isTargetString = 1;
        }

        if (isTargetString) {
            //Save the last rule and initialize fresh rule
            saveRule(list_of_vertices, current_rule);
            char *rule_name = (char *) GetNext(parsed_string);
            DeleteNode(parsed_string, parsed_string->next);
            current_rule = initializeRule(rule_name, parsed_string, 1);
        } else {
            //Add this command to rule object
            command *parsed_command = parseCommand(parsed_string);
            AddNode(current_rule->commands, parsed_command);
        }
    }
    saveRule(list_of_vertices, current_rule);
    return list_of_vertices;
}

vertex *FindRuleVertex(list_node *list, char *current_rule_name) {
    vertex *vert;
    rule *vert_data;

    while (list != NULL) {
        vert = (vertex *) GetNext(list);
        if (vert != NULL) {
            vert_data = (rule *) vert->data;
            if (vert_data != NULL && strcmp(vert_data->target_name, current_rule_name) == 0)
                return vert;
        }
        list = list->next;
    }
    return NULL;
}

void saveRule(list_node *list, rule *current_rule) {
    if (current_rule == NULL) return;

    vertex *vert = FindRuleVertex(list, current_rule->target_name);
    if (vert == NULL) {
        vert = CreateVertex(current_rule);
        AddNode(list, vert);
        current_rule->isInitialized = 1;
    } else {
        //A vertex with same rule name exists
        rule *vert_data = (rule *) vert->data;
        //Check if it was initialized or it was eager loaded without initialization
        if (vert_data->isInitialized)
            DuplicateRuleError();
        //In case of uninitialized rule, set the appropriate dependencies and commands
        vert_data->dependencies = current_rule->dependencies;
        vert_data->commands = current_rule->commands;
        vert_data->isInitialized = 1;
    }

    //For new or eager loaded rule,we will have to set the dependencies represented as edges
    setDependenciesOfRule(list, vert, current_rule->dependencies);
}

void setDependenciesOfRule(list_node *list, vertex *currentVertex, list_node *dependencies) {
    char *dependency_name;
    vertex *dependency_vertex;
    rule *dependency_rule;
    while (dependencies != NULL) {
        dependency_name = (char *) GetNext(dependencies);
        if (dependency_name != NULL) {
            //Check if a rule exists with this name. If so then add an edge to that vertex from current vertex
            //If not then eagerly load an uninitialized rule into a vertex and create edge to this vertex
            dependency_vertex = FindRuleVertex(list, dependency_name);
            if (dependency_vertex == NULL) {
                dependency_rule = initializeRule(dependency_name, NULL, 0);
                dependency_vertex = CreateVertex(dependency_rule);
                AddNode(list, dependency_vertex);
            }
            AddEdge(currentVertex, dependency_vertex);
        }
        dependencies = dependencies->next;
    }
}

rule *initializeRule(char *target_name, list_node *dependencies, int isInitialized) {
    rule *new_rule = malloc(sizeof(rule));
    new_rule->target_name = target_name;
    new_rule->dependencies = dependencies;
    new_rule->commands = CreateLinkedList();
    new_rule->isInitialized = isInitialized;
    return new_rule;
}

command *parseCommand(list_node *list) {
    command *parsed_command = malloc(sizeof(command));
    parsed_command->command_args = list;

    char *data = (char *) GetNext(list); //This is the command name
    parsed_command->command_name = data;
    DeleteNode(list, list->next);

    return parsed_command;
}