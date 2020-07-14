#ifndef _REGISTER_H_
#define _REGISTER_H_

#include <boolean.h>
#include <metadata.h>

//Union para Dados
typedef union {
    char *c;
    double d;
    float f;
    int i;
} Data;

//Struct para registros
typedef struct {
    int key;
    Data **data;
    int registerSize;
    int elementCounter;
} Register;

//Funções para manusear registros
void readRegisterFromString(Register **reg, char *registerLine, Metadata *metadata);
boolean writeRegister(Register *, Metadata *);
void freeRegister(Register **);

#endif