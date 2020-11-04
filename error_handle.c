#include "error_handle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void WaitPIDError(int line_index, char *line_str, int errorNo) {
    fprintf(stderr, ERROR_TEMPLATE, line_index, strerror(errorNo), line_str);
    exit(EXIT_FAILURE);
}

void BufferOverflowError(int index, char *line) {
    fprintf(stderr, ERROR_TEMPLATE, index, "line size exceeds max buffer size", line);
    exit(EXIT_FAILURE);
}

void NullByteInLineError(int index, char *line) {
    fprintf(stderr, ERROR_TEMPLATE, index, "null byte found while parsing the line", line);
    exit(EXIT_FAILURE);
}

void CycleInGraphError(int index, char *line) {
    fprintf(stderr, ERROR_TEMPLATE, index, "cycle detected in makefile", line);
    exit(EXIT_FAILURE);
}

void TargetParsingError(int index, char *line, char *error_msg) {
    fprintf(stderr, ERROR_TEMPLATE, index, error_msg, line);
    exit(EXIT_FAILURE);
}

void InvalidCommandInMakefileError(int index, char *line) {
    fprintf(stderr, ERROR_TEMPLATE, index, "command is not part of any target", line);
    exit(EXIT_FAILURE);
}

void DuplicateRuleError(int index, char *line) {
    fprintf(stderr, ERROR_TEMPLATE, index, "Duplicate rule found", line);
    exit(EXIT_FAILURE);
}

void CommandExecutionFailedError(int index, char *line, int errNo) {
    fprintf(stderr, "%d: command could not be executed. Error message is - %s: %s\n", index, strerror(errNo), line);
    exit(EXIT_FAILURE);
}

void RuleNotFoundError(char *invalid_rule) {
    fprintf(stderr, "%s: invalid target name\nExiting!\n", invalid_rule);
    exit(EXIT_FAILURE);
}

void InvalidTargetDependencyError(int line_index, char *line_str, char *dep_name) {
    fprintf(stderr, "%d: Invalid target dependency of %s: %s\n", line_index, dep_name, line_str);
    exit(EXIT_FAILURE);
}

void FileOpenError(char *filename, int errorNo) {
    fprintf(stderr, "%s: File could not be opened. Error message is - %s\n", filename, strerror(errorNo));
    exit(EXIT_FAILURE);
}

void FileNotFoundError(char *filename, int line_index, char *line_str) {
    if (line_str == NULL)
        fprintf(stderr, "File not found error occured for %s\n", filename);
    else
        fprintf(stderr, "%d: File not found error occurred for %s: %s\n", line_index, filename, line_str);

    exit(EXIT_FAILURE);
}

void StatError(int line_index, char *line_str, char *filename, int errorNo) {
    fprintf(stderr, "%d: Error finding file stats of %s. Error Message is %s: %s\n", line_index, filename,
            strerror(errorNo), line_str);
    exit(EXIT_FAILURE);
}

void InvalidArgumentsError(char *error_msg) {
    fprintf(stderr, "%s\n", error_msg);
    exit(EXIT_FAILURE);
}

void ValidateMemoryAllocationError(void *arg) {
    if (arg == NULL) {
        fprintf(stderr, "Memory could not be allocated using malloc/calloc. Exiting!\n");
        exit(EXIT_FAILURE);
    }
}
