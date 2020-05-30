#ifndef _REGISTER_H_
#define _REGISTER_H_

typedef union {
    char *c;
    double d;
    float f;
    int i;
} Data;

typedef struct {
    int key;
    Data **data;
} Register;

typedef struct {
    int registersSize;
    Register **registers;
} RegistersOperator;

#endif