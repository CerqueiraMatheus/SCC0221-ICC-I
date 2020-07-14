#ifndef _METADATA_H_
#define _METADATA_H_

//Struct para campos dos Metadados
typedef struct {
    char *name;
    char *type;
} Field;

//Struct para os Metadados
typedef struct {
    char *fileName;
    char *keyName;
    char *keyType;
    int fieldsLength;
    Field **fields;
} Metadata;

//Funções para manusear Metadados
Metadata *readMetaData(const char *);
void freeMetadata(Metadata *);

#endif