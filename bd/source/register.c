#include "register.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "extractor.h"
#include "index.h"
#include "metadata.h"

/**
 * 
 * Arquivo responsável por manusear registros
 * 
 * */

//Atribui a string ao tipo correspondente do registro descrito nos metadados
void populateRegisterDataFromString(char *data, int position, Register *reg, Metadata *metadata) {
    //Inicializa o dado
    reg->data[position] = calloc(1, sizeof(Data));

    //Caso int
    if (!strcmp(metadata->fields[position]->type, "int"))
        reg->data[position]->i = atoi(data);

    //Caso float
    else if (!strcmp(metadata->fields[position]->type, "float"))
        reg->data[position]->f = (float)atof(data);

    //Caso double
    else if (!strcmp(metadata->fields[position]->type, "double"))
        reg->data[position]->d = atof(data);

    //Caso char *
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

//Lê os arquivos de um registro a partir de uma string separada por vírgulas
void readRegisterFromString(Register **reg, char *registerLine, Metadata *metadata) {
    //Inicializa o registro
    *reg = (Register *)calloc(1, sizeof(Register));
    (*reg)->elementCounter = 0;
    (*reg)->data = NULL;

    //Converte o ponteiro em vetor para evitar
    //undef behavior no strtol
    char convertedLine[strlen(registerLine) + 1];
    for (int i = 0; i <= strlen(registerLine); i++) convertedLine[i] = registerLine[i];
    char *currentToken = strtok(convertedLine, ",");

    //Seta a chave do registro
    (*reg)->key = atoi(currentToken);

    //Separa e armazena os dados separados
    //por vírgula
    char **tokens = NULL;
    char *aux = NULL;
    do {
        aux = strtok(NULL, ",");
        if (aux) {
            tokens = (char **)realloc(tokens, ++((*reg)->elementCounter) * sizeof(char *));
            tokens[((*reg)->elementCounter) - 1] = aux;
        }
    } while (aux);

    //Insere os dados no registro
    (*reg)->data = (Data **)realloc((*reg)->data, sizeof(Data *) * ((*reg)->elementCounter));
    for (int i = 0; i < ((*reg)->elementCounter); i++) {
        populateRegisterDataFromString(tokens[i], i, *reg, metadata);
    }

    //Libera os ponteiros utilizados
    free(tokens);
    free(registerLine);
}

//Lê um dado binário a partir de um tipo nos metadados
//e armazena num registro
void populateRegisterDataFromFile(FILE *registersFile, int position, Register *reg, Metadata *metadata) {
    //Inicializa o dado
    reg->data[position] = calloc(1, sizeof(Data));

    //Caso int
    if (!strcmp(metadata->fields[position]->type, "int"))
        fread(&reg->data[position]->i, sizeof(int), 1, registersFile);

    //Caso float
    else if (!strcmp(metadata->fields[position]->type, "float"))
        fread(&reg->data[position]->f, sizeof(float), 1, registersFile);

    //Caso double
    else if (!strcmp(metadata->fields[position]->type, "double"))
        fread(&reg->data[position]->i, sizeof(double), 1, registersFile);

    //Caso char *
    else {
        int stringSize = getNumberInsideBracketsFromCharArray(metadata->fields[position]->type);
        reg->data[position]->c = calloc(
            stringSize,
            sizeof(char));
        fread(reg->data[position]->c, sizeof(char), stringSize, registersFile);
    }

    //Incrementa o contador de elementos do registro
    reg->elementCounter++;
}

//Lê um registro de um arquivo binário a partir de um index
boolean readRegisterFromFile(Metadata *metadata, Register **reg, int index) {
    //Recebe o resultado do offset pelo index
    int offset = findOffsetByIndex(index);

    //Caso exista (!= -1)
    if (offset != -1) {
        *reg = calloc(1, sizeof(Register));

        //Abre o registro a partir do offset
        FILE *registersFile = fopen(metadata->fileName, "rb");
        fseek(registersFile, offset, SEEK_CUR);

        //Inicializa o registro
        (*reg)->elementCounter = 0;
        fread(&(*reg)->key, sizeof(int), 1, registersFile);

        //Inicializa os dados do registro
        (*reg)->data = calloc(metadata->fieldCounter, sizeof(Data *));
        for (int i = 0; i < metadata->fieldCounter; i++)
            populateRegisterDataFromFile(registersFile, i, *reg, metadata);

        //Fecha o arquivo e retorna sucesso
        fclose(registersFile);
        return TRUE;
    } else
        //Retorna falha
        return FALSE;
}

//Escreve o dado do registro de acordo com o formato e retorna seu tamanho
void writeRegisterDataToFile(Metadata *metadata, Register *reg, FILE *registersFile, int position) {
    //Caso int
    if (!strcmp(metadata->fields[position]->type, "int"))
        fwrite(&reg->data[position]->i, sizeof(int), 1, registersFile);

    //Caso float
    else if (!strcmp(metadata->fields[position]->type, "float"))
        fwrite(&reg->data[position]->f, sizeof(float), 1, registersFile);

    //Caso double
    else if (!strcmp(metadata->fields[position]->type, "double"))
        fwrite(&reg->data[position]->d, sizeof(double), 1, registersFile);

    //Caso char *
    else {
        fwrite(reg->data[position]->c,
               sizeof(char),
               getNumberInsideBracketsFromCharArray(metadata->fields[position]->type),
               registersFile);
    }
}

//Escreve um registro no arquivo de registros e retorna seu tamanho
boolean writeRegisterToFile(Register *reg, Metadata *metadata) {
    //Abre o arquivo
    FILE *registersFile = fopen(metadata->fileName, "ab");
    if (!registersFile) return FALSE;

    //Escreve a chave do registro
    fwrite(&reg->key, sizeof(int), 1, registersFile);

    //Escreve dado por dado de um registro
    for (int i = 0; i < reg->elementCounter; i++) {
        writeRegisterDataToFile(metadata, reg, registersFile, i);
    }

    //Fecha o arquivo
    fclose(registersFile);

    return TRUE;
}

//Retorna o tamanho do registro de Metadata
int getMetadataRegistersSize(Metadata *metadata) {
    //Referente a "key", sempre int
    int size = sizeof(int);

    //Aos campos do registro
    for (int i = 0; i < metadata->fieldCounter; i++) {
        if (!strcmp(metadata->fields[i]->type, "int"))
            size += sizeof(int);

        else if (!strcmp(metadata->fields[i]->type, "float"))
            size += sizeof(float);

        else if (!strcmp(metadata->fields[i]->type, "double"))
            size += sizeof(double);

        else {
            size += getNumberInsideBracketsFromCharArray(metadata->fields[i]->type);
        }
    }
    return size;
}

//Exibe os elementos de um registro de acordo com seu tipo
//definido no arquivo de metadados
void printRegister(Register *reg, Metadata *metadata) {
    printf("%s: %d\n", metadata->keyName, reg->key);

    for (int i = 0; i < metadata->fieldCounter; i++) {
        printf("%s: ", metadata->fields[i]->name);

        if (!strcmp(metadata->fields[i]->type, "int"))
            printf("%d\n", reg->data[i]->i);

        else if (!strcmp(metadata->fields[i]->type, "float"))
            printf("%.2f\n", reg->data[i]->f);

        else if (!strcmp(metadata->fields[i]->type, "double"))
            printf("%.2lf\n", reg->data[i]->d);

        else
            printf("%s\n", reg->data[i]->c);
    }

    printf("\n");
}

//Libera um Registro
void freeRegister(Register **reg, Metadata *metadata) {
    char aux[5];

    //Percorre os elementos de um registro
    for (int i = 0; i < (*reg)->elementCounter; i++) {
        //Constrói uma string com apenas os 4 primeiros char de uma string
        for (int j = 0; j < 4; j++) aux[j] = metadata->fields[i]->type[j];
        aux[4] = '\0';

        //Se for char *, desaloca
        if (!strcmp(aux, "char"))
            free((*reg)->data[i]->c);

        //Libera o dados do registro
        free((*reg)->data[i]);
    }
    //Libera o array de dados e o registro
    free((*reg)->data);
    free(*reg);
}