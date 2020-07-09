#include <boolean.h>
#include <extractor.h>
#include <metadata.h>
#include <register.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 
 * Arquivo responsável por manusear registros
 * 
 * */

void setRegisterData(char *data, int position, Register *reg, Metadata *metadata);
int writeRegisterData(Metadata *metadata, Register *reg, FILE *registersFile, int position);

//Lê os arquivos de um registro
Register *readRegister(char *registerLine, Metadata *metadata) {
    Register *reg = NULL;
    char *aux = NULL;
    char *output = NULL;
    int registerIndex = 0, wordIndex = 0;

    reg = (Register *)malloc(sizeof(Register));

    aux = strtok(registerLine, ",");
    printf("%s\n", aux);

    while (aux) {
        int i = 0;
        for (; wordIndex < strlen(aux); wordIndex++, i++) {
            output = (char *)realloc(output, sizeof(char) * (i + 1));
            output[i] = aux[wordIndex];
        }
        output = (char *)realloc(output, wordIndex);
        output[i - 1] = '\0';

        if (!registerIndex) {
            printf("ENTROU");
            reg->key = atoi(output);
            free(output);
            reg->elementCounter = 1;
            reg->registerSize = sizeof(int);
            reg->elementCounter = 1;
            reg->data = (Data **)malloc(sizeof(Data *) * (reg->elementCounter));
        } else {
            reg->data = (Data **)realloc(reg->data, sizeof(Data *) * 300);
            setRegisterData(aux, wordIndex, reg, metadata);
        }
        aux = strtok(NULL, ",");
        registerIndex++;
    }
}

//Escreve um registro no arquivo de registros e retorna seu tamanho
int writeRegister(Register *reg, Metadata *metadata) {
    //Abre o arquivo
    FILE *registersFile = fopen(metadata->fileName, "w");
    if (!registersFile) {
        printf("deu ruim :(");
        return FALSE;
    }
    int size = 0;

    printf("REGELEMENTS::: %d\n", reg->elementCounter);

    //Escreve dado por dado de um registro
    for (int i = 0; i < reg->elementCounter; i++) {
        size += writeRegisterData(metadata, reg, registersFile, i);
    }

    //Fecha o arquivo
    fclose(registersFile);
    return size;
}

//Libera um Registro
void freeRegister(Register *reg) {
    for (int j = 0; j < reg->registerSize; j++) {
        free(reg->data[j]);
    }
    free(reg);
}

//Atribui a string ao tipo correspondente do registro descrito nos metadados
void setRegisterData(char *data, int position, Register *reg, Metadata *metadata) {
    if (strcmp(metadata->fields[position]->type, "int")) {
        reg->data[position]->i = atoi(data);
        reg->registerSize += sizeof(int);
        free(data);
    } else if (strcmp(metadata->fields[position]->type, "float")) {
        reg->data[position]->f = (float)atof(data);
        reg->registerSize = sizeof(float);
        free(data);
    } else if (strcmp(metadata->fields[position]->type, "double")) {
        reg->data[position]->d = atof(data);
        reg->registerSize = sizeof(double);
        free(data);
    } else {
        reg->registerSize = sizeof(data);
        reg->data[position]->c = data;
    }
}

//Escreve o dado do registro de acordo com o formato e retorna seu tamanho
int writeRegisterData(Metadata *metadata, Register *reg, FILE *registersFile, int position) {
    if (strcmp(metadata->fields[position]->type, "int")) {
        fprintf(registersFile, "%d", reg->data[position]->i);
        return sizeof(int);
    } else if (strcmp(metadata->fields[position]->type, "float")) {
        fprintf(registersFile, "%f", reg->data[position]->f);
        return sizeof(float);
    } else if (strcmp(metadata->fields[position]->type, "double")) {
        fprintf(registersFile, "%lf", reg->data[position]->d);
        return sizeof(double);
    } else {
        reg->data[position]->c = (char *)realloc(reg->data[position]->c,
                                                 getNumberInsideBracketsFromCharArray(metadata->fields[position]->type));
        fputs(reg->data[position]->c, registersFile);
        return sizeof(sizeof(reg->data[position]->c));
    }
}