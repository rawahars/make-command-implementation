/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#include "execution_engine.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>

/**
 * These are the internal methods used in this module
 * */
int post_order_graph_traversal(list_node *all_vertices, list_node *visited, vertex *curr_vertex, char *parent_target);

int getLastModificationTime(char *filename, int line_index, char *line_str);

void execute_rule(rule *current_rule);

int execute_command(command *cmd);

int execute(char **args, int line_index, char *cmd_str);

/**
 * This method is used to execute the execution graph passed to this method starting from the given rule
 * */
void ExecuteExecutionGraph(list_node *list_vertices, char *execution_rule) {
    vertex *rule_vertex;
    if (execution_rule == NULL) { //if we haven't provided any rule then execute the first rule
        rule_vertex = (vertex *) GetNext(list_vertices);
    } else { //otherwise find the rule in the graph
        rule_vertex = FindRuleVertex(list_vertices, execution_rule);
        if (rule_vertex == NULL) //if rule is not found in graph then raise an error
            RuleNotFoundError(execution_rule);
    }

    //Execute the graph from above vertex
    list_node *executed_rules = CreateLinkedList();
    post_order_graph_traversal(list_vertices, executed_rules, rule_vertex, NULL);
    DeleteLinkedList(executed_rules);
}

/**
 * This method is used to traverse and execute the execution graph in a post order manner
 * We return 1 if the rule was out of date else we return 0
 * */
int post_order_graph_traversal(list_node *all_vertices, list_node *visited, vertex *curr_vertex, char *parent_target) {
    rule *curr_rule = (rule *) GetData(curr_vertex);
    if (curr_rule == NULL) return 0;
    //If the rule vertex has been visited then return
    if (FindRuleVertex(visited, curr_rule->target_name) != NULL) return 0;
    int isThisRuleOutOfDate = 0;

    //isInitialized = 0 means that this is a file vertex or an invalid rule (not present in the graph)
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

    //if the rule vertex doesn't have any dependencies then it is out of date
    if (GetNext(edges) == NULL) {
        isThisRuleOutOfDate = 1;
    }

    //for all the dependencies, we will recursively call post_order_graph_traversal
    //also, if we found that all the edges were files then we mark current rule as out of date
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

    //if this rule was out of date then we execute its commands
    if (isThisRuleOutOfDate)
        execute_rule(curr_rule);
    //add current rule vertex to visited list
    AddNode(visited, curr_vertex);

    return isThisRuleOutOfDate;
}

/**
 * This method is used to get the last modification time of a time.
 * In case of any error such as stat error or file not found error, we raise appropriate error.
 * */
int getLastModificationTime(char *filename, int line_index, char *line_str) {
    if (access(filename, F_OK) == -1) //if file is not present
        FileNotFoundError(filename, line_index, line_str);

    struct stat file_stat;
    int retval = stat(filename, &file_stat); //find file stats
    if (retval == -1) //in case of getting stats, we raise error
        StatError(line_index, line_str, filename, errno);

    int last_modification = file_stat.st_mtime;
    return last_modification;
}

/**
 * This method takes in a rule and executes all the commands associated with the rule
 * */
void execute_rule(rule *current_rule) {
    list_node *cmd_list = current_rule->commands;
    command *cmd;
    int retval;

    //Iterate over all the commands of the rule and execute them one by one by calling execute_command method
    while (cmd_list != NULL) {
        cmd = (command *) GetNext(cmd_list);
        if (cmd != NULL) {
            retval = execute_command(cmd);
            if (retval != 0) { //In case of execution failure, we raise an error
                CommandExecutionFailedError(cmd->cmd_index, cmd->cmd_string, retval);
            }
        }
        cmd_list = cmd_list->next;
    }
}

/**
 * This method takes in a command struct and executes it.
 * We parse the command parameters into appropriate format and call the execute method on it.
 * */
int execute_command(command *cmd) {
    list_node *head = cmd->command_args;
    int len = GetLength(head);
    char **cmd_args = malloc(sizeof(char *) * (len + 2));
    ValidateMemoryAllocationError(cmd_args);

    //convert command object into an array of strings. We pass this to execute method.
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

    //call execute with the generated parameters
    int ret_status = execute(cmd_args, cmd->cmd_index, cmd->cmd_string);
    free(cmd_args);
    return ret_status;
}

/**
 * This method is used to execute the given command.
 * We execute the command in the child process. In case of any errors, we raise appropriate error.
 * */
int execute(char **args, int line_index, char *cmd_str) {
    int pid, status;
    if ((pid = fork()) < 0) {
        CommandExecutionFailedError(line_index, cmd_str, errno);
    }

    if (!pid) { //Child
        //Execute command using execvp
        if (execvp(args[0], args) < 0)
            _exit(errno);
        _exit(1);
    }

    //Wait on all child process
    int retVal = waitpid(-1, &status, 0);
    if (retVal == -1)
        WaitPIDError(line_index, cmd_str, errno);

    return WEXITSTATUS(status);
}
