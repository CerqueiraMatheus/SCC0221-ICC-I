#ifndef _EXTRACTOR_H_
#define _EXTRACTOR_H_

#include <stdio.h>
#include <boolean.h>

#define FILENAME "filename"
#define KEYNAME "key-name"
#define KEYTYPE "key-type"
#define FIELDNAME "field-name"
#define FIELDTYPE "field-type"

char *findValueFromName(const char *, char *);
char *getLineFromFile(FILE *);

#endif