/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#include "error_handle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This method is called when we want to raise error for waitpid method failure
 * */
void WaitPIDError(int line_index, char *line_str, int errorNo) {
    fprintf(stderr, ERROR_TEMPLATE, line_index, strerror(errorNo), line_str);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error for buffer overflow
 * */
void BufferOverflowError(int index, char *line) {
    fprintf(stderr, ERROR_TEMPLATE, index, "line size exceeds max buffer size", line);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error for receiving null byte in the string while parsing
 * */
void NullByteInLineError(int index, char *line) {
    fprintf(stderr, ERROR_TEMPLATE, index, "null byte found while parsing the line", line);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error after detecting cycle in the execution graph
 * */
void CycleInGraphError(int index, char *line) {
    fprintf(stderr, ERROR_TEMPLATE, index, "cycle detected in makefile", line);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error due to failure while parsing the target string.
 * Such errors include - First character is not tab or character, etc
 * */
void TargetParsingError(int index, char *line, char *error_msg) {
    fprintf(stderr, ERROR_TEMPLATE, index, error_msg, line);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error for a command without target in makefile
 * */
void InvalidCommandInMakefileError(int index, char *line) {
    fprintf(stderr, ERROR_TEMPLATE, index, "command is not part of any target", line);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error for a duplicate target in the makefile
 * */
void DuplicateRuleError(int index, char *line) {
    fprintf(stderr, ERROR_TEMPLATE, index, "Duplicate rule found", line);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error for a failed execution of command
 * */
void CommandExecutionFailedError(int index, char *line, int errNo) {
    fprintf(stderr, "%d: command could not be executed. Error message is - %s: %s\n", index, strerror(errNo), line);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error for a missing rule in the makefile
 * */
void RuleNotFoundError(char *invalid_rule) {
    fprintf(stderr, "%s: invalid target name\nExiting!\n", invalid_rule);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error to indicate invalid target dependency in the makefile.
 * One such example is non-existent file in the dependency
 * */
void InvalidTargetDependencyError(int line_index, char *line_str, char *dep_name) {
    fprintf(stderr, "%d: Invalid target dependency of %s: %s\n", line_index, dep_name, line_str);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error since file could not be opened
 * */
void FileOpenError(char *filename, int errorNo) {
    fprintf(stderr, "%s: File could not be opened. Error message is - %s\n", filename, strerror(errorNo));
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error since the file doesn't exist
 * */
void FileNotFoundError(char *filename, int line_index, char *line_str) {
    if (line_str == NULL)
        fprintf(stderr, "File not found error occured for %s\n", filename);
    else
        fprintf(stderr, "%d: File not found error occurred for %s: %s\n", line_index, filename, line_str);

    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error since we could not open the file stats
 * */
void StatError(int line_index, char *line_str, char *filename, int errorNo) {
    fprintf(stderr, "%d: Error finding file stats of %s. Error Message is %s: %s\n", line_index, filename,
            strerror(errorNo), line_str);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error for invalid arguments passed to main method
 * */
void InvalidArgumentsError(char *error_msg) {
    fprintf(stderr, "%s\n", error_msg);
    exit(EXIT_FAILURE);
}

/**
 * This method is called when we want to raise error for failure during memory allocation
 * */
void ValidateMemoryAllocationError(void *arg) {
    if (arg == NULL) {
        fprintf(stderr, "Memory could not be allocated using malloc/calloc. Exiting!\n");
        exit(EXIT_FAILURE);
    }
}
