#include "execution_engine.h"
#include <stdlib.h>
#include <unistd.h>
#include <sysexits.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>

int post_order_graph_traversal(list_node *all_vertices, list_node *visited, vertex *curr_vertex, char *parent_target);

int getLastModificationTime(char *filename);

void execute_rule(rule *current_rule);

int execute_command(command *cmd);

int execute(char **args);

void ExecuteExecutionGraph(list_node *list_vertices, char *execution_rule) {
    vertex *rule_vertex;
    if (execution_rule == NULL) {
        //Execute the first rule
        rule_vertex = (vertex *) GetNext(list_vertices);
    } else {
        //Find the rule in the graph
        rule_vertex = FindRuleVertex(list_vertices, execution_rule);
        if (rule_vertex == NULL)
            RuleNotFoundError();
    }
    //Execute the graph from rule_vertex
    list_node *executed_rules = CreateLinkedList();
    post_order_graph_traversal(list_vertices, executed_rules, rule_vertex, NULL);
    DeleteLinkedList(executed_rules);
}

//Returns if the target was out of date. 1-Yes 0-No
int post_order_graph_traversal(list_node *all_vertices, list_node *visited, vertex *curr_vertex, char *parent_target) {
    rule *curr_rule = (rule *) GetData(curr_vertex);
    if (curr_rule == NULL) return 0;
    if (FindRuleVertex(visited, curr_rule->target_name) != NULL) return 0;
    int isThisRuleOutOfDate = 0;

    //Means that it is possibly a file or invalid rule entry
    if (!curr_rule->isInitialized) {
        if (FindRuleVertex(all_vertices, curr_rule->target_name) == NULL) {
            //Means it is a file. Check for that and raise error if not present.
            if (access(curr_rule->target_name, F_OK) == -1)
                InvalidTargetDependencyError(curr_rule->target_name);
            else {
                //Check if time stamp of the file is greater than target.
                //return from here since it is a file.
                if (getLastModificationTime(curr_rule->target_name) < getLastModificationTime(parent_target))
                    return 1;
                else
                    return 0;
            }
        } else {
            //Uninitialized vertex in graph means that it is an invalid target
            InvalidTargetDependencyError(curr_rule->target_name);
        }
    }

    vertex *next_vertex;
    list_node *edges = curr_vertex->edges;

    if (GetNext(edges) == NULL) {
        isThisRuleOutOfDate = 1;
    }

    while (edges != NULL) {
        next_vertex = (vertex *) GetNext(edges);
        if (next_vertex != NULL) {
            if (post_order_graph_traversal(all_vertices, visited, next_vertex, curr_rule->target_name))
                isThisRuleOutOfDate = 1;
        }
        edges = edges->next;
    }

    if (isThisRuleOutOfDate)
        execute_rule(curr_rule);
    AddNode(visited, curr_vertex);

    return isThisRuleOutOfDate;
}

int getLastModificationTime(char *filename) {
    if (access(filename, F_OK) == -1)
        FileNotFoundError(filename);

    struct stat file_stat;
    stat(filename, &file_stat);
    int last_modification = file_stat.st_mtime;
    return last_modification;
}

void execute_rule(rule *current_rule) {
    list_node *cmd_list = current_rule->commands;
    command *cmd;
    int retval;

    while (cmd_list != NULL) {
        cmd = (command *) GetNext(cmd_list);
        if (cmd != NULL) {
            retval = execute_command(cmd);
            if (retval != 0) {
                CommandExecutionFailedError(retval);
            }
        }
        cmd_list = cmd_list->next;
    }
}

int execute_command(command *cmd) {
    list_node *head = cmd->command_args;
    int len = GetLength(head);
    char **cmd_args = malloc(sizeof(char *) * (len + 2));

    int index = 1;
    char *curr_cmd_arg;
    cmd_args[0] = cmd->command_name;
    while (head != NULL) {
        curr_cmd_arg = (char *) GetNext(head);
        if (curr_cmd_arg != NULL) {
            cmd_args[index++] = curr_cmd_arg;
        }
        head = head->next;
    }
    cmd_args[len + 1] = NULL;
    int ret_status = execute(cmd_args);
    free(cmd_args);
    return ret_status;
}

int execute(char **args) {
    int pid, status;
    if ((pid = fork()) < 0) {
        CommandExecutionFailedError(EX_OSERR);
    }

    if (!pid) { //Child
        //Execute arguments
        if (execvp(args[0], args + 1) < 0)
            exit(errno);
        else
            exit(1);
    }

    //Wait on child
    waitpid(pid, &status, 0);

    return status;
}
