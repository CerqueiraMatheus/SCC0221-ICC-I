#ifndef _REGISTER_H_
#define _REGISTER_H_

#include <boolean.h>
#include <metadata.h>

typedef union {
    char *c;
    double d;
    float f;
    int i;
} Data;

typedef struct {
    int key;
    Data **data;
    int registerSize;
    int elementCounter;
} Register;

Register *readRegister(char *, Metadata *);
int writeRegister(Register *, Metadata *);
void freeRegister(Register *);

#endif