/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "makefile_parser.h"

/**
 * These are the internal methods used in this module
 * */
rule *initializeRule(char *target_name, list_node *dependencies, int isInitialized, int index, char *line);

void saveRule(list_node *list, rule *current_rule);

void setDependenciesOfRule(list_node *list, vertex *currentVertex, list_node *dependencies);

command *parseCommand(list_node *list, int index, char *line);

int isVertexInList(vertex *curr_vertex, list_node *list);

void removeVertexFromList(list_node *list, vertex *curr_vertex);

int dfs(vertex *curr_vertex, list_node *global_list, list_node *curr_list);

/**
 * This method is used to parse the makefile into an execution graph
 * */
list_node *ParseMakefile(FILE *file) {
    list_node *list_of_vertices = CreateLinkedList();
    char *line;
    int index = 0;
    list_node *parsed_string;
    int isTargetString = 0;
    rule *current_rule = NULL;

    //iterate till the end of file is reached and parse each line into a rule
    while (feof(file) == 0) {
        index++;
        //read line from the file
        line = ReadLine(file, index);

        //Based on whether the line is a target or command, we call the corresponding method of text_parser
        //In case of errors, we raise appropriate error
        if (line == NULL) continue;
        else if (line[0] == '\t') {
            parsed_string = ParseCommandString(line);
            if (current_rule == NULL) { //Command appeared before any target
                InvalidCommandInMakefileError(index, line);
            }
            isTargetString = 0;
        } else {
            parsed_string = ParseTargetString(line, index);
            isTargetString = 1;
        }

        //for each target rule, save the previous one and initialize a fresh rule
        if (isTargetString) {
            saveRule(list_of_vertices, current_rule);
            char *rule_name = (char *) GetNext(parsed_string);
            DeleteNode(parsed_string, parsed_string->next); // rule_name is not present in the dependencies list
            current_rule = initializeRule(rule_name, parsed_string, 1, index, line);
        } else {
            //In case of command, we add that to the current running rule
            command *parsed_command = parseCommand(parsed_string, index, line);
            AddNode(current_rule->commands, parsed_command);
        }
    }
    //At the end, we save the absolute last rule and return the execution graph
    saveRule(list_of_vertices, current_rule);
    return list_of_vertices;
}

/**
 * This method is used to find if the given rule_name vertex is present in the graph
 * */
vertex *FindRuleVertex(list_node *list, char *current_rule_name) {
    vertex *vert;
    rule *vert_data;

    //Iterate over all the nodes of the graph and check if the rule_name matches the given rule_name
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

/**
 * This method can be used to print the execution graph.
 * We usually use this for diagnostic purposes
 * */
void PrintExecutionGraph(list_node *current_node) {
    vertex *node = (vertex *) GetNext(current_node);
    if (node == NULL) return;
    rule *curr_rule = (rule *) GetData(node);
    printf("Current Node: %s\t . Initialization Status: %d\n", curr_rule->target_name, curr_rule->isInitialized);
    printf("Dependencies for %s are: ", curr_rule->target_name);

    list_node *dependencies = node->edges;
    vertex *dependent_vertex;
    rule *dep_rule;
    //For the current vertex, find its dependencies and iterate over them to find the dependency name
    while (dependencies != NULL) {
        dependent_vertex = (vertex *) GetNext(dependencies);
        if (dependent_vertex != NULL) {
            dep_rule = (rule *) GetData(dependent_vertex);
            printf("%s\t", dep_rule->target_name);
        }
        dependencies = dependencies->next;
    }
    printf("\n\n");

    //Go to next node and print it as well
    PrintExecutionGraph(current_node->next);
}

/**
 * This is an internal method which can be used to save a rule to the current graph
 * */
void saveRule(list_node *list, rule *current_rule) {
    if (current_rule == NULL) return;

    //If a vertex with given name exists then add current rule's data to that else create a new vertex with the rule
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
            DuplicateRuleError(current_rule->target_index, current_rule->target_str);
        //In case of uninitialized rule, set the appropriate dependencies and commands
        vert_data->dependencies = current_rule->dependencies;
        vert_data->commands = current_rule->commands;
        vert_data->isInitialized = 1;
        vert_data->target_index = current_rule->target_index;
        vert_data->target_str = current_rule->target_str;
    }

    //For new or eager loaded rule,we will have to set the dependencies represented as edges
    setDependenciesOfRule(list, vert, current_rule->dependencies);
}

/**
 * This is an internal method which is used to set all the dependencies of a given rule.
 * If the corresponding rule vertex is not present then we will eager load it with initializationStatus as 0.
 * */
void setDependenciesOfRule(list_node *list, vertex *currentVertex, list_node *dependencies) {
    rule *current_rule = (rule *) (GetData(currentVertex));
    char *dependency_name;
    vertex *dependency_vertex;
    rule *dependency_rule;
    //We create edges to all the dependencies of the given rule
    while (dependencies != NULL) {
        dependency_name = (char *) GetNext(dependencies);
        if (dependency_name != NULL) {
            //Check if a rule exists with this name. If so then add an edge to that vertex from current vertex
            //If not then eagerly load an uninitialized rule into a vertex and create edge to this vertex
            dependency_vertex = FindRuleVertex(list, dependency_name);
            if (dependency_vertex == NULL) {
                dependency_rule = initializeRule(dependency_name, NULL, 0, current_rule->target_index,
                                                 current_rule->target_str);
                dependency_vertex = CreateVertex(dependency_rule);
                AddNode(list, dependency_vertex);
            }
            AddEdge(currentVertex, dependency_vertex);
        }
        dependencies = dependencies->next;
    }
}

/**
 * This method is used to initialize a new rule with the given parameters
 * */
rule *initializeRule(char *target_name, list_node *dependencies, int isInitialized, int index, char *line) {
    rule *new_rule = malloc(sizeof(rule));
    ValidateMemoryAllocationError(new_rule);
    new_rule->target_name = target_name;
    new_rule->dependencies = dependencies;
    new_rule->commands = CreateLinkedList();
    new_rule->isInitialized = isInitialized;
    new_rule->target_index = index;
    new_rule->target_str = line;
    return new_rule;
}

/**
 * This method is used to parse a command as a list to a command struct
 * */
command *parseCommand(list_node *list, int index, char *line) {
    command *parsed_command = malloc(sizeof(command));
    ValidateMemoryAllocationError(parsed_command);
    parsed_command->command_args = list;

    //Add command name to separate field and args to the list
    char *data = (char *) GetNext(list); //This is the command name
    parsed_command->command_name = data;
    DeleteNode(list, list->next);//Delete the command name from the list

    parsed_command->cmd_index = index;
    parsed_command->cmd_string = line;

    return parsed_command;
}

/*
 * This method checks if the created vertex has been populated 
 * in the list of nodes created while traversing the graph 
 * to detect if cycle is present
 */
int isVertexInList(vertex *curr_vertex, list_node *list) {
    if (list->next == NULL) return 0;
    list_node *temp = list->next;
    rule *current_vertex_rule = (rule *) curr_vertex->data;
    while (temp != NULL) {
        vertex *list_vertex = (vertex *) temp->data;
        rule *list_vertex_rule = (rule *) list_vertex->data;
        if (strcmp(current_vertex_rule->target_name, list_vertex_rule->target_name) == 0) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

/*
 * If the vertex has been checked and there is no cycle detected 
 * on any of its edges, the vertex is removed from the list of
 * nodes
 */
void removeVertexFromList(list_node *list, vertex *curr_vertex) {
    list_node *temp = list->next;
    rule *current_vertex_rule = (rule *) curr_vertex->data;
    while (temp != NULL) {
        vertex *list_vertex = (vertex *) temp->data;
        rule *list_vertex_rule = (rule *) list_vertex->data;
        if (strcmp(current_vertex_rule->target_name, list_vertex_rule->target_name) == 0) {
            list->next = temp->next;
            free(temp);
            break;
        }
        list = temp;
        temp = list->next;
    }
}

/*
 * This function does depth-first-traversal on every vertex and
 * detects presence of cycle on the vertext and its edges
 */
int dfs(vertex *curr_vertex, list_node *global_list, list_node *curr_list) {
    if (curr_vertex == NULL) return 0;
    if (isVertexInList(curr_vertex, global_list)) {
        return 0;
    } else if (isVertexInList(curr_vertex, curr_list)) {
        rule *curr_rule = (rule *) (GetData(curr_vertex));
        CycleInGraphError(curr_rule->target_index, curr_rule->target_str);
        return 1;
    }
    list_node *edges = curr_vertex->edges;
    AddNode(curr_list, curr_vertex);
    vertex *next_vertex;
    while (edges != NULL) {
        next_vertex = (vertex *) GetNext(edges);
        if (dfs(next_vertex, global_list, curr_list) == 1) {
            return 1;
        }
        edges = edges->next;
    }
    if (isVertexInList(curr_vertex, global_list) == 0) {
        AddNode(global_list, curr_vertex);
    }
    removeVertexFromList(curr_list, curr_vertex);
    return 0;
}

/*
 * This is the handler function to detect cycle in the graph
 */
int DetectCycleInGraph(list_node *start) {
    list_node *temp = start->next;
    list_node *global_list = CreateLinkedList();
    while (temp != NULL) {
        list_node *curr_list = CreateLinkedList();
        vertex *v = (vertex *) temp->data;
        if (dfs(v, global_list, curr_list) == 1) {
            return 1;
        }
        temp = temp->next;
        DeleteLinkedList(curr_list);
    }
    DeleteLinkedList(global_list);
    return 0;
}