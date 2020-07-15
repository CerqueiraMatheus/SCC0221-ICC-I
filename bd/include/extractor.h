#ifndef _EXTRACTOR_H_
#define _EXTRACTOR_H_

#include <stdio.h>

#include "boolean.h"

//Campos estáticos dos arquivos
#define FILENAME "filename"
#define KEYNAME "key-name"
#define KEYTYPE "key-type"
#define FIELDNAME "field-name"
#define FIELDTYPE "field-type"

//Opções de funcionalidade
#define INSERT 1
#define INDEX 2
#define SEARCH 3
#define EXIT 4

//Funções gerais
char *findValueFromName(char *, char *);
char *getLineFromFile(FILE *);
int getOptionFromFile(FILE *);
int getNumberInsideBracketsFromCharArray(char *inputLine);

#endif