#include "index.h"

#include <stdio.h>
#include <stdlib.h>

#include "boolean.h"
#include "metadata.h"
#include "register.h"

/**
 * 
 * Arquivo responsável por manusear os indexes dos registros
 * 
 * */

//Organiza o array de index de registros por Bubble Sort
void sortRegisterIndexArray(RegisterIndex **registersIndex, int size) {
    for (int i = 1; i < size; i++) {
        for (int j = size - 1; j >= i; j--) {
            if (registersIndex[j - 1]->key > registersIndex[j]->key) {
                RegisterIndex *aux = registersIndex[j - 1];
                registersIndex[j - 1] = registersIndex[j];
                registersIndex[j] = aux;
            }
        }
    }
}

//Busca um elemento num vetor de index de registros (retorna -1 se não encontrado)
int binarySearchIndex(RegisterIndex **registerIndexes, int begin, int end, int value) {
    if (end >= begin) {
        int middle = (begin + end) / 2;

        //Caso 1: valor encontrado
        if (value == registerIndexes[middle]->key) return middle;

        //Caso 2: busca pela "direita"
        if (value > registerIndexes[middle]->key) {
            return binarySearchIndex(registerIndexes, middle + 1, end, value);
        } else {
            //Caso 3: busca pela "esquerda"
            return binarySearchIndex(registerIndexes, begin, middle - 1, value);
        }
    } else
        return -1;  //Caso 4: valor não encontrado
}

//Lê um index a partir de um arquivo binári
boolean readIndex(RegisterIndex ***registerIndexes, int *indexCounter) {
    //Abre o binário
    FILE *registersIndexFile = fopen(INDEX_FILENAME, "rb");

    //Se for aberto
    if (registersIndexFile) {
        //Extrai o tamanho do arquivo e retorna
        //o ponteiro ao início
        fseek(registersIndexFile, 0, SEEK_END);
        int fileSize = ftell(registersIndexFile);
        fseek(registersIndexFile, 0, SEEK_SET);

        //Percorre todos os index e armazena na lista
        for (*indexCounter = 0; ftell(registersIndexFile) != fileSize; (*indexCounter)++) {
            *registerIndexes = realloc(*registerIndexes, ((*indexCounter) + 1) * sizeof(RegisterIndex *));
            (*registerIndexes)[(*indexCounter)] = malloc(sizeof(RegisterIndex));
            fread(&(*registerIndexes)[(*indexCounter)]->key, sizeof(int), 1, registersIndexFile);
            fread(&(*registerIndexes)[(*indexCounter)]->offset, sizeof(int), 1, registersIndexFile);
        }

        //Fecha o arquivo e retorna sucesso
        fclose(registersIndexFile);
        return TRUE;
    }
    //Se não for, retorna falso
    return FALSE;
}

//Libera uma lista de index de registro
void freeRegisterIndexList(RegisterIndex ***registerIndexes, int indexCounter) {
    for (int i = 0; i < indexCounter; i++)
        free((*registerIndexes)[i]);
    free(*registerIndexes);
}

//Retorna um offset a partir de um inteiro
//num arquivo binário
int findOffsetByIndex(int index) {
    RegisterIndex **registerIndexes = NULL;
    int indexCounter = 0, resultIndex = 0, resultOffset = 0;

    //Se for possível ler o arquivo binário
    if (readIndex(&registerIndexes, &indexCounter)) {
        //Busca o index
        resultIndex = binarySearchIndex(registerIndexes, 0, indexCounter, index);

        //Se encontrado, retorna o offset
        //se não, -1
        if (resultIndex == -1)
            resultOffset = -1;
        else
            resultOffset = registerIndexes[resultIndex]->offset;

        //Libera os indexes
        freeRegisterIndexList(&registerIndexes, indexCounter);

        return resultOffset;
    }
    return -1;
}

//Gera o arquivo de index
void writeIndex(Metadata *metadata) {
    int registerSize = getMetadataRegistersSize(metadata);
    RegisterIndex **registerIndexes = NULL;

    //Abre o arquivo de registros
    FILE *registersFile = fopen(metadata->fileName, "rb");

    //Se for possível abrir
    if (registersFile) {
        //Extrai o tamanho do arquivo
        fseek(registersFile, 0, SEEK_END);
        int fileSize = ftell(registersFile);
        fseek(registersFile, 0, SEEK_SET);

        //Percorre o arquivo de registros
        boolean fileEnd = FALSE;
        int regCounter;
        for (regCounter = 0; !fileEnd; regCounter++) {
            //Inicializa o registro
            registerIndexes = realloc(registerIndexes, (regCounter + 1) * sizeof(RegisterIndex *));
            registerIndexes[regCounter] = malloc(sizeof(RegisterIndex));

            //Lê o index e offset
            fread(&registerIndexes[regCounter]->key, sizeof(int), 1, registersFile);
            registerIndexes[regCounter]->offset = ftell(registersFile) - sizeof(int);

            //Se a próxima não superar o tamanho do arquivo
            //passa ao próximo registro. Se superar, ativa
            //o "fim de arquivo"
            if (registerIndexes[regCounter]->offset + registerSize < fileSize)
                fseek(registersFile, registerSize - sizeof(int), SEEK_CUR);
            else
                fileEnd = TRUE;
        }

        //Ordena os indexes
        sortRegisterIndexArray(registerIndexes, regCounter);

        //Escreve os indexes no arquivo de indexes
        FILE *registersIndex = fopen(INDEX_FILENAME, "wb");
        for (int i = 0; i < regCounter; i++) {
            fwrite(&registerIndexes[i]->key, sizeof(int), 1, registersIndex);
            fwrite(&registerIndexes[i]->offset, sizeof(int), 1, registersIndex);
            free(registerIndexes[i]);
        }
        //Libera o ponteiro
        free(registerIndexes);

        //Fecha os arquivos
        fclose(registersFile);
        fclose(registersIndex);
    }
}