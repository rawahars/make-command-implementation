#include "error_handle.h"
#include <stdio.h>
#include <stdlib.h>

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