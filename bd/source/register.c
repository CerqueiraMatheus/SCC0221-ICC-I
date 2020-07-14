#include "register.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "extractor.h"
#include "metadata.h"

/**
 * 
 * Arquivo responsável por manusear registros
 * 
 * */
void setRegisterData(char *data, int position, Register *reg, Metadata *metadata);
void writeRegisterData(Metadata *metadata, Register *reg, FILE *registersFile, int position);

//Lê os arquivos de um registro
void readRegisterFromString(Register **reg, char *registerLine, Metadata *metadata) {
    *reg = (Register *)calloc(1, sizeof(Register));
    (*reg)->elementCounter = 0;
    (*reg)->data = NULL;

    char convertedLine[strlen(registerLine) + 1];
    for (int i = 0; i <= strlen(registerLine); i++) convertedLine[i] = registerLine[i];
    char *currentToken = strtok(convertedLine, ",");

    (*reg)->key = atoi(currentToken);

    char **tokens = NULL;
    char *aux = NULL;
    do {
        aux = strtok(NULL, ",");
        if (aux) {
            tokens = (char **)realloc(tokens, ++((*reg)->elementCounter) * sizeof(char *));
            tokens[((*reg)->elementCounter) - 1] = aux;
        }
    } while (aux);

    (*reg)->data = (Data **)realloc((*reg)->data, sizeof(Data *) * ((*reg)->elementCounter));
    for (int i = 0; i < ((*reg)->elementCounter); i++) {
        setRegisterData(tokens[i], i, *reg, metadata);
    }
    free(tokens);
    free(registerLine);
}

//Escreve um registro no arquivo de registros e retorna seu tamanho
boolean writeRegister(Register *reg, Metadata *metadata) {
    //Abre o arquivo
    FILE *registersFile = fopen(metadata->fileName, "ab");
    if (!registersFile) return FALSE;

    //Escreve a chave do registro
    fwrite(&reg->key, sizeof(int), 1, registersFile);

    //Escreve dado por dado de um registro
    for (int i = 0; i < reg->elementCounter; i++) {
        writeRegisterData(metadata, reg, registersFile, i);
    }

    //Fecha o arquivo
    fclose(registersFile);

    return TRUE;
}

//Atribui a string ao tipo correspondente do registro descrito nos metadados
void setRegisterData(char *data, int position, Register *reg, Metadata *metadata) {
    //Inicializa o dado
    reg->data[position] = calloc(1, sizeof(Data));

    if (!strcmp(metadata->fields[position]->type, "int"))
        reg->data[position]->i = atoi(data);

    else if (!strcmp(metadata->fields[position]->type, "float"))
        reg->data[position]->f = (float)atof(data);

    else if (!strcmp(metadata->fields[position]->type, "double"))
        reg->data[position]->d = atof(data);

    //Último caso: caracteres
    else {
        reg->data[position]->c = calloc(
            getNumberInsideBracketsFromCharArray(metadata->fields[position]->type),
            sizeof(char));

        //Remove as aspas
        int i = 2;
        for (; i < strlen(data) - 1; i++)
            reg->data[position]->c[i - 2] = data[i];
        reg->data[position]->c[i - 1] = '\0';
    }
}

//Escreve o dado do registro de acordo com o formato e retorna seu tamanho
void writeRegisterData(Metadata *metadata, Register *reg, FILE *registersFile, int position) {
    if (!strcmp(metadata->fields[position]->type, "int"))
        fwrite(&reg->data[position]->i, sizeof(int), 1, registersFile);

    else if (!strcmp(metadata->fields[position]->type, "float"))
        fwrite(&reg->data[position]->f, sizeof(float), 1, registersFile);

    else if (!strcmp(metadata->fields[position]->type, "double"))
        fwrite(&reg->data[position]->d, sizeof(double), 1, registersFile);

    else {
        fwrite(reg->data[position]->c,
               sizeof(char),
               getNumberInsideBracketsFromCharArray(metadata->fields[position]->type),
               registersFile);
        free(reg->data[position]->c);
    }
}

//Libera um Registro
void freeRegister(Register **reg) {
    for (int i = 0; i < (*reg)->elementCounter; i++)
        free((*reg)->data[i]);
    free((*reg)->data);
    free(*reg);
}