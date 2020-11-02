#ifndef ASSIGNMENT3_ERROR_HANDLE_H
#define ASSIGNMENT3_ERROR_HANDLE_H

void BufferOverflowError();
void NullByteInLineError();
void CycleInGraphError();
void TargetParsingError();
void InvalidCommandInMakefileError();
void DuplicateRuleError();
void CommandExecutionFailedError(int exitCode);
void RuleNotFoundError();
void InvalidTargetDependencyError(char* error_msg);
#endif
