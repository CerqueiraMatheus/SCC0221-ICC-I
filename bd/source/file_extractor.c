#include <boolean.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_BUFFER 2048

char *findValueFromName(const char *inputLine, char *name) {
    char *output = 0;
    int nameSize = 0, index = 0;
    boolean wasFound = FALSE;

    char *newName = malloc(sizeof(char) * (strlen(name) + 3));
    strcpy(newName, name);
    strcat(newName, ": ");
    strstr(inputLine, newName);
    nameSize = strlen(newName);

    for (index = 0; nameSize < strlen(inputLine); nameSize++, index++) {
        output = (char *)realloc(output, sizeof(char) * (index + 1));
        output[index] = inputLine[nameSize];
    }

    output = (char *)realloc(output, sizeof(char) * (index + 1));
    output[index] = '\0';

    free(newName);

    return output;
}

char *getLineFromFile(FILE *inputFile) {
    char *line = 0;
    int pos = 0;

    do {
        if (pos % INPUT_BUFFER == 0) {
            line = (char *)realloc(line,
                                   (pos / INPUT_BUFFER + 1) * INPUT_BUFFER);
        }
        line[pos] = (char)fgetc(inputFile);
    } while (line[pos++] != '\n' && !feof(inputFile));

    line[pos - 1] = '\0';
    line = (char *)realloc(line, pos);

    return line;
}