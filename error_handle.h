/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#ifndef ASSIGNMENT3_ERROR_HANDLE_H
#define ASSIGNMENT3_ERROR_HANDLE_H

#define ERROR_TEMPLATE "%d: %s: %s\n"

void StatError(int line_index, char *line_str, char *filename, int errorNo);

void WaitPIDError(int line_index, char *line_str, int errorNo);

void FileNotFoundError(char *filename, int line_index, char *line_str);

void FileOpenError(char *filename, int errorNo);

void BufferOverflowError(int index, char *line);

void NullByteInLineError(int index, char *line);

void CycleInGraphError(int index, char *line);

void TargetParsingError(int index, char *line, char *error_msg);

void InvalidCommandInMakefileError(int index, char *line);

void DuplicateRuleError(int index, char *line);

void CommandExecutionFailedError(int index, char *line, int errNo);

void RuleNotFoundError(char *invalid_rule);

void InvalidTargetDependencyError(int line_index, char *line_str, char *dep_name);

void ValidateMemoryAllocationError(void *arg);

void InvalidArgumentsError(char *error_msg);

#endif
