#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "text_parser.h"

int filter(char *line);

int isFirstCharacterLetterOrTab(char *line);

int hasOnlySpaces(char *line);

char *extractLineFromFile(FILE *file);

char *reallocateString(char *buffer, int len);

void split(list_node *head, char *str, int startIndex, int maxLen);

char *ReadLine(FILE *file) {
    char *line = extractLineFromFile(file);
    if (!filter(line)) {
        return NULL;
    }
    return line;
}

list_node *ParseTargetString(char *line) {
    list_node *target = CreateLinkedList();
    int len = strlen(line);
    int index = 0;

    // Extract the target name from the target line
    char *currStr = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    ValidateMemoryAllocationError(currStr);
    for (; index < len; index++) {
        if (line[index] == ' ' || line[index] == '\t')
            TargetParsingError();
        if (line[index] == ':') {
            currStr[index] = '\0';
            break;
        }
        currStr[index] = line[index];
    }
    if (index == len) {// No : found in the target string
        TargetParsingError();
    }
    char *str = reallocateString(currStr, index);
    AddNode(target, str);

    // Extract dependencies of the target by splitting them on whitespaces
    split(target, line, index + 1, len);

    return target;
}

list_node *ParseCommandString(char *line) {
    list_node *command_list = CreateLinkedList();
    int len = strlen(line);
    split(command_list, line, 1, len);
    return command_list;
}

int filter(char *line) {
    if (strlen(line) == 0 || hasOnlySpaces(line) || line[0] == '#' || !isFirstCharacterLetterOrTab(line)) {
        return 0;
    }
    return 1;
}

int isFirstCharacterLetterOrTab(char *line) {
    int len = strlen(line);
    if (isalnum(line[0]))
        return 1;
    else if (len > 1 && line[0] == '\t') {
        if (isalnum(line[1]))
            return 1;
        else
            TargetParsingError();
    }
    return 0;
}

int hasOnlySpaces(char *line) {
    int len = strlen(line);
    for (int i = 0; i < len; i++) {
        if (line[i] != ' ') return 0;
    }
    return 1;
}

char *extractLineFromFile(FILE *file) {
    char *buffer = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    ValidateMemoryAllocationError(buffer);
    int len = 0;
    char ch;

    while (1) {
        ch = fgetc(file);

        if (len >= MAX_BUFFER_SIZE) { // During buffer overflow exit with error
            free(buffer);
            BufferOverflowError();
        } else if (ch == '\0') { // If a null byte is found in line then exit with error
            NullByteInLineError();
        } else if (ch == EOF || ch == '\n') {
            buffer[len++] = '\0';
            break;
        } else {
            buffer[len++] = ch;
        }
    }

    char *final_str = reallocateString(buffer, len);
    return final_str;
}

char *reallocateString(char *buffer, int len) {
    char *final_str = malloc(sizeof(char) * len);
    ValidateMemoryAllocationError(final_str);
    strcpy(final_str, buffer);
    free(buffer);
    return final_str;
}

void split(list_node *head, char *str, int initialIndex, int maxLen) {
    int index;
    char *currStr = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    ValidateMemoryAllocationError(currStr);
    char *newStr;
    for (index = initialIndex; index < maxLen; index++) {
        if (str[index] == ' ' || str[index] == '\t') {
            if ((index - initialIndex) == 0) {
                initialIndex++;
            } else {
                currStr[(index - initialIndex)] = '\0';
                newStr = reallocateString(currStr, (index - initialIndex));
                AddNode(head, newStr);
                currStr = malloc(sizeof(char) * MAX_BUFFER_SIZE);
                ValidateMemoryAllocationError(currStr);
                initialIndex = index + 1;
            }
        } else {
            currStr[(index - initialIndex)] = str[index];
        }
    }

    if ((index - initialIndex) != 0) {
        currStr[maxLen - initialIndex] = '\0';
        newStr = reallocateString(currStr, (index - initialIndex));
        AddNode(head, newStr);
    }
}