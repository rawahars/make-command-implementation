#ifndef ASSIGNMENT3_ERROR_HANDLE_H
#define ASSIGNMENT3_ERROR_HANDLE_H

#define ERROR_TEMPLATE "%d: %s: %s\n"

void StatError(int line_index, char *line_str, char *filename, int errorNo);

void WaitPIDError(int line_index, char *line_str, int errorNo);

void FileNotFoundError(char *filename, int line_index, char *line_str);

void FileOpenError(char *filename, int errorNo);

void BufferOverflowError(int index, char *line);

void NullByteInLineError(int index, char *line);

void CycleInGraphError();

void TargetParsingError();

void InvalidCommandInMakefileError();

void DuplicateRuleError();

void CommandExecutionFailedError(int exitCode);

void RuleNotFoundError();

void InvalidTargetDependencyError(char *error_msg);

/**
 * These are generic errors which are independent of the makefile. Therefore we are not following the standard format.
 * */
void ValidateMemoryAllocationError(void *arg);

void InvalidArgumentsError(char *error_msg);

#endif
