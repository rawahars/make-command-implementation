#include "execution_engine.h"
#include <stdlib.h>
#include <unistd.h>
#include <sysexits.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>

int post_order_graph_traversal(list_node *all_vertices, list_node *visited, vertex *curr_vertex, char *parent_target);

int getLastModificationTime(char *filename, int line_index, char *line_str);

void execute_rule(rule *current_rule);

int execute_command(command *cmd);

int execute(char **args, int line_index, char *cmd_str);

void ExecuteExecutionGraph(list_node *list_vertices, char *execution_rule) {
    vertex *rule_vertex;
    if (execution_rule == NULL) {
        //Execute the first rule
        rule_vertex = (vertex *) GetNext(list_vertices);
    } else {
        //Find the rule in the graph
        rule_vertex = FindRuleVertex(list_vertices, execution_rule);
        if (rule_vertex == NULL)
            RuleNotFoundError(execution_rule);
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
        //Check if it is a file. Check for that and raise error if not present.
        if (access(curr_rule->target_name, F_OK) == -1)
            InvalidTargetDependencyError(curr_rule->target_index, curr_rule->target_str, curr_rule->target_name);
        else {
            //Check if time stamp of the file is greater than target.
            //return from here since it is a file.
            int curr_file_timestamp = getLastModificationTime(curr_rule->target_name, curr_rule->target_index,
                                                              curr_rule->target_str);
            if (access(parent_target, F_OK) == -1)
                return 1;
            int parent_target_timestamp = getLastModificationTime(parent_target, curr_rule->target_index,
                                                                  curr_rule->target_str);
            if (curr_file_timestamp > parent_target_timestamp)
                return 1;
            else
                return 0;
        }
    }

    int are_all_edges_target = 1;
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
            if (!((rule *) GetData(next_vertex))->isInitialized)
                are_all_edges_target = 0;
        }
        edges = edges->next;
    }

    if (are_all_edges_target) isThisRuleOutOfDate = 1;

    if (isThisRuleOutOfDate)
        execute_rule(curr_rule);
    AddNode(visited, curr_vertex);

    return isThisRuleOutOfDate;
}

int getLastModificationTime(char *filename, int line_index, char *line_str) {
    if (access(filename, F_OK) == -1)
        FileNotFoundError(filename, line_index, line_str);

    struct stat file_stat;
    int retval = stat(filename, &file_stat);
    if (retval == -1)
        StatError(line_index, line_str, filename, errno);
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
                CommandExecutionFailedError(cmd->cmd_index, cmd->cmd_string, retval);
            }
        }
        cmd_list = cmd_list->next;
    }
}

int execute_command(command *cmd) {
    list_node *head = cmd->command_args;
    int len = GetLength(head);
    char **cmd_args = malloc(sizeof(char *) * (len + 2));
    ValidateMemoryAllocationError(cmd_args);

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
    int ret_status = execute(cmd_args, cmd->cmd_index, cmd->cmd_string);
    free(cmd_args);
    return ret_status;
}

int execute(char **args, int line_index, char *cmd_str) {
    int pid, status;
    if ((pid = fork()) < 0) {
        CommandExecutionFailedError(line_index, cmd_str, errno);
    }

    if (!pid) { //Child
        //Execute arguments
        if (execvp(args[0], args) < 0)
            _exit(errno);
        _exit(1);
    }

    //Wait on child
    int retVal = waitpid(-1, &status, 0);
    if (retVal == -1)
        WaitPIDError(line_index, cmd_str, errno);

    return WEXITSTATUS(status);
}
