#include "error_handle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ValidateMemoryAllocationError(void *arg) {
    if (arg == NULL) {
        fprintf(stderr, "Memory could not be allocated using malloc/calloc. Exiting!\n");
        exit(EXIT_FAILURE);
    }
}

void StatError(int errorNo) {
    fprintf(stderr, "Error getting file stats. Error description: %s. \nExiting!\n", strerror(errorNo));
    exit(EXIT_FAILURE);
}

void WaitPIDError(int errorNo) {
    fprintf(stderr, "Error while waiting for child process. Error msg: %s.\nExiting!\n", strerror(errorNo));
    exit(EXIT_FAILURE);
}

void InvalidArgumentsError(char *error_msg) {
    printf("%s", error_msg);
    exit(-1);
}

void FileNotFoundError(char *error_msg) {
    printf("%s", error_msg);
    exit(-1);
}

void BufferOverflowError() {
    printf("BufferOverflowError\n");
    exit(-1);
}

void NullByteInLineError() {
    printf("NullByteInLineError\n");
    exit(-1);
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