#include <boolean.h>
#include <metadata.h>
#include <register_operator.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * 
 * Pressupõe-se que o formato da string seja passado como
 * "|key|, |data|, |data|, |data|" 
 * 
 * */

Register *readRegister(char *registerLine, FILE *registerFile, Metadata *metadata) {
    Register *reg = NULL;
    char *aux = NULL;
    char *output = NULL;
    int registerIndex = 0, wordIndex;

    reg = (Register *)malloc(sizeof(Register));

    aux = strtok(registerLine, ",");
    while (aux) {
        for (wordIndex = 0; wordIndex < strlen(aux); wordIndex++) {
            output = (char *)realloc(output, wordIndex);
            output[wordIndex - 1] = aux[wordIndex];
        }
        output = (char *)realloc(output, wordIndex + 1);
        output[wordIndex] = '\0';

        if (!registerIndex) {
            reg->key = atoi(output);
            free(output);
        } else {
            stringToMetadataType(aux, wordIndex, reg, metadata);
        }
        aux = strtok(NULL, ",");
        registerIndex = 0;
    }
}

//Atribui a string ao tipo correspondente do registro descrito nos metadados
void stringToMetadataType(char *data, int position, Register *reg, Metadata *metadata) {
    if (strcmp(metadata->fields[position], "int")) {
        reg->data[position]->i = atoi(data);
        free(data);
    } else if (strcmp(metadata->fields[position], "float")) {
        reg->data[position]->f = (float)atof(data);
        free(data);
    } else if (strcmp(metadata->fields[position], "double")) {
        reg->data[position]->d = atof(data);
        free(data);
    } else {
        reg->data[position]->c = data;
    }
}