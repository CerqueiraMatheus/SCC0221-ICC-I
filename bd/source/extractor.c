#include "extractor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"

#define INPUT_BUFFER 2048

/**
 * 
 * Arquivo respnsável por manusear arquivos e strings gerais
 * 
 * */

//Retorna um valor a partir do nome [in: ("nome: valor") out: "valor"].
char *findValueFromName(char *inputLine, char *name) {
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
    free(inputLine);

    return output;
}

//Retorna uma linha a partir de um arquivo.
char *getLineFromFile(FILE *inputFile) {
    char *line = NULL;
    int pos = 0;

    do {
        if (pos % INPUT_BUFFER == 0) {
            line = (char *)realloc(line,
                                   (pos / INPUT_BUFFER + 1) * INPUT_BUFFER);
        }
        char aux = (char)fgetc(inputFile);
        if (aux != '\r') line[pos++] = aux;
    } while (line[pos - 1] != '\n' && !feof(inputFile));

    line[pos - 1] = '\0';

    return line;
}

//Retorna o número de uma string no formato "char[int]"
int getNumberInsideBracketsFromCharArray(char *inputLine) {
    char aux[strlen(inputLine) + 1];
    for (int i = 0; i <= strlen(inputLine); i++) aux[i] = inputLine[i];
    inputLine = strtok(aux, "]");
    int auxNumber = atoi(inputLine + 5);
    return auxNumber;
}

//Retorna uma opção através de uma linha de arquivo
int getOptionFromFile(FILE *inputFile) {
    char *line = 0;
    int pos = 0, output = 0;

    do {
        if (pos % INPUT_BUFFER == 0)
            line = (char *)realloc(line,
                                   (pos / INPUT_BUFFER + 1) * INPUT_BUFFER);
        line[pos++] = (char)fgetc(inputFile);
    } while (line[pos - 1] != ' ' && line[pos - 1] != '\n' && line[pos - 1] != '\r' && !feof(inputFile));

    line[pos - 1] = '\0';

    if (!strcmp(line, "insert"))
        output = INSERT;

    else if (!strcmp(line, "index"))
        output = INDEX;

    else if (!strcmp(line, "search"))
        output = SEARCH;

    else
        output = EXIT;

    free(line);

    return output;
}