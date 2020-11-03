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
    fprintf(stderr, ERROR_TEMPLATE, index, "line size exceeds max buffer size", line);
    exit(EXIT_FAILURE);
}

void CycleInGraphError() {
    printf("CycleInGraphError\n");
    exit(-1);
}

void TargetParsingError() {
    printf("TargetParsingError\n");
    exit(-1);
}

void InvalidCommandInMakefileError() {
    printf("InvalidCommandInMakefileError\n");
    exit(-1);
}

void DuplicateRuleError() {
    printf("DuplicateRuleError\n");
    exit(-1);
}

void CommandExecutionFailedError(int exitCode) {
    printf("CommandExecutionFailedError\n");
    exit(-1);
}

void RuleNotFoundError() {
    printf("RuleNotFoundError\n");
    exit(-1);
}

void InvalidTargetDependencyError(char *error_msg) {
    printf("InvalidTargetDependencyError: %s\n", error_msg);
    exit(-1);
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
