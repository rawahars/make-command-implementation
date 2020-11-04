/**
 * @author Harsh Rawat, harsh-rawat, hrawat2
 * @author Sidharth Gurbani, gurbani, gurbani
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "text_parser.h"

/**
 * These are all the internal methods used in this module
 * */
int filter(char *line, int index);

int isFirstCharacterLetterOrTab(char *line, int index);

int hasOnlySpaces(char *line);

char *extractLineFromFile(FILE *file, int index);

char *reallocateString(char *buffer, int len);

void split(list_node *head, char *str, int startIndex, int maxLen);

/**
 * This method is used to read a line from the file.
 * Filtering is performed based on the project criteria such as comments, null bytes, buffer size, etc
 * */
char *ReadLine(FILE *file, int index) {
    //Extract the line from the file
    char *line = extractLineFromFile(file, index);
    //Perform filter checks on the extracted string
    if (!filter(line, index)) {
        return NULL;
    }
    return line;
}

/**
 * This method is used to parse a string which is in the target format i.e. target_name: <dep list>
 * The result is returned as a linked list wherein first element is the target name and remaining are its dependencies.
 * */
list_node *ParseTargetString(char *line, int line_index) {
    list_node *target = CreateLinkedList();
    int len = strlen(line);
    int index = 0;
    int whitespace_encountered = 0;

    // Extract the target name from the target line
    char *currStr = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    ValidateMemoryAllocationError(currStr);
    for (; index < len; index++) {
        //if we encounter whitespaces after target_name then ignore those
        if (line[index] == ' ' || line[index] == '\t') {
            whitespace_encountered++;
            continue;
        }
        //If we get : then break as we got the target name
        if (line[index] == ':') {
            currStr[index - whitespace_encountered] = '\0';
            break;
        }
        //If we have a whitespace between target name such as "target 1: <dep list>" then raise error
        if (whitespace_encountered > 0 && (line[index] != ' ' && line[index] != '\t'))
            TargetParsingError(line_index, line, "invalid format of line");

        currStr[index] = line[index];
    }

    if (index == len) {// No : found in the target string so raise an error
        TargetParsingError(line_index, line, "invalid format of line");
    }
    //If all seems correct then reallocate the string to its size and add it to the linked list
    char *str = reallocateString(currStr, index);
    AddNode(target, str);

    // Extract dependencies of the target by splitting them on whitespaces
    split(target, line, index + 1, len);

    return target;
}

/**
 * This method is used to parse the string which is in command format i.e. "\tcommand_name <command_args>"
 * We basically just split the string on whitespaces
 * The result is returned as a linked list
 * */
list_node *ParseCommandString(char *line) {
    list_node *command_list = CreateLinkedList();
    int len = strlen(line);
    split(command_list, line, 1, len);
    return command_list;
}

/**
 * This method is used to filter the line based on project criteria
 * */
int filter(char *line, int index) {
    if (strlen(line) == 0 || hasOnlySpaces(line) || line[0] == '#' || !isFirstCharacterLetterOrTab(line, index)) {
        return 0;
    }
    return 1;
}

/**
 * This is an internal method which is used to check if the first character is a character or a tab
 * */
int isFirstCharacterLetterOrTab(char *line, int index) {
    if ((!isspace(line[0])) || line[0] == '\t')
        return 1;
    else
        TargetParsingError(index, line, "first character should be either a character or a tab");
    return 0;
}

/**
 * This method is used to verify if the line only has spaces
 * */
int hasOnlySpaces(char *line) {
    int len = strlen(line);
    for (int i = 0; i < len; i++) {
        if (line[i] != ' ') return 0;
    }
    return 1;
}

/**
 * This method is used to extract a line from a file. We perform buffer overflow and null byte check as we extract the line.
 * */
char *extractLineFromFile(FILE *file, int index) {
    char *buffer = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    ValidateMemoryAllocationError(buffer);
    int len = 0;
    char ch;

    while (1) {
        ch = fgetc(file);

        if (len >= MAX_BUFFER_SIZE) { // During buffer overflow exit with error
            buffer[MAX_BUFFER_SIZE - 1] = '\0';
            BufferOverflowError(index, buffer);
        } else if (ch == '\0') { // If a null byte is found in line then exit with error
            buffer[len] = '\0';
            NullByteInLineError(index, buffer);
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

/**
 * This method is used to reallocate a string to its size
 * */
char *reallocateString(char *buffer, int len) {
    char *final_str = malloc(sizeof(char) * len);
    ValidateMemoryAllocationError(final_str);
    strcpy(final_str, buffer);
    free(buffer);
    return final_str;
}

/**
 * This method is used to split the line on whitespaces
 * */
void split(list_node *head, char *str, int initialIndex, int maxLen) {
    int index;
    char *currStr = malloc(sizeof(char) * MAX_BUFFER_SIZE);
    ValidateMemoryAllocationError(currStr);
    char *newStr;
    //Iterate over the length of the string starting from the given index
    for (index = initialIndex; index < maxLen; index++) {
        if (str[index] == ' ' || str[index] == '\t') { //ignore whitespaces
            if ((index - initialIndex) == 0) {
                initialIndex++;
            } else {
                //If we encounter a new whitespace and we have an old string then add it to list
                currStr[(index - initialIndex)] = '\0';
                newStr = reallocateString(currStr, (index - initialIndex));
                AddNode(head, newStr);
                //Initialize new string for current string
                currStr = malloc(sizeof(char) * MAX_BUFFER_SIZE);
                ValidateMemoryAllocationError(currStr);
                initialIndex = index + 1;
            }
        } else {
            currStr[(index - initialIndex)] = str[index];
        }
    }

    //verify if the last string was a valid split which should be added to the list
    if ((index - initialIndex) != 0) {
        currStr[maxLen - initialIndex] = '\0';
        newStr = reallocateString(currStr, (index - initialIndex));
        AddNode(head, newStr);
    }
}