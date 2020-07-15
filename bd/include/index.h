#ifndef _INDEX_H_
#define _INDEX_H_

#include "metadata.h"

//Nome constante do arquivo gerado
#define INDEX_FILENAME "registros.idx"

//Struct para o Index de um Registro
typedef struct {
    int key;
    int offset;
} RegisterIndex;

//Funções para manusear Index de registros
void writeIndex(Metadata *metadata);
int findOffsetByIndex(int index);

#endif