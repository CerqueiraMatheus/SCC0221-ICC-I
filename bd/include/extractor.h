#ifndef _EXTRACTOR_H_
#define _EXTRACTOR_H_

#include <boolean.h>
#include <stdio.h>

#define FILENAME "filename"
#define KEYNAME "key-name"
#define KEYTYPE "key-type"
#define FIELDNAME "field-name"
#define FIELDTYPE "field-type"

#define INSERT 1
#define INDEX 2
#define SEARCH 3
#define EXIT 4

char *findValueFromName(const char *, char *);
char *getLineFromFile(FILE *);
int getOptFromFile(FILE *);
int getNumberInsideBracketsFromCharArray(const char *);

#endif