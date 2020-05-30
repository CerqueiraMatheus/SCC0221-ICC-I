#ifndef _METADATA_H_
#define _METADATA_H_

typedef struct {
    char *name;
    char *type;
} Field;

typedef struct {
    char *fileName;
    char *keyName;
    char *keyType;
    int fieldsLenght;
    Field **fields;
} Metadata;

Metadata *readMetaData(const char *);
void freeMetadata(Metadata *);

#endif