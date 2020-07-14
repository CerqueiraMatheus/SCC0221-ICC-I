#include "metadata.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "extractor.h"

/**
 * 
 * Arquivo responsável por ler e liberar Metadata
 * 
 * */
void readHeader(FILE *metadataFile, Metadata *metadata);
void readBody(FILE *metadataFile, Metadata *metadata);

//Lê e atribui Metadata
Metadata *readMetaData(const char *fileName) {
    //Abre o arquivo de metadados
    FILE *metadataFile = fopen(fileName, "r");
    if (!metadataFile) return NULL;

    //Lê e atribui os campos
    Metadata *metadata = (Metadata *)malloc(sizeof(Metadata));
    readHeader(metadataFile, metadata);
    readBody(metadataFile, metadata);

    //Fecha o arquivo
    fclose(metadataFile);

    return metadata;
}

//Lê o cabeçalho de Metadata
void readHeader(FILE *metadataFile, Metadata *metadata) {
    metadata->fileName = findValueFromName(getLineFromFile(metadataFile), FILENAME);
    metadata->keyName = findValueFromName(getLineFromFile(metadataFile), KEYNAME);
    metadata->keyType = findValueFromName(getLineFromFile(metadataFile), KEYTYPE);
}

//Lê o corpo de Metadata
void readBody(FILE *metadataFile, Metadata *metadata) {
    boolean isNull = FALSE;
    metadata->fieldsLength = 0;

    Field **listField = NULL;
    for (int i = 0; !isNull; i++) {
        listField = (Field **)realloc(listField, sizeof(Field *) * (i + 1));
        Field *field = (Field *)malloc(sizeof(Field));
        field->name = findValueFromName(getLineFromFile(metadataFile), FIELDNAME);
        field->type = findValueFromName(getLineFromFile(metadataFile), FIELDTYPE);

        if (strlen(field->name) && strlen(field->type)) {
            listField[i] = field;
            metadata->fieldsLength++;
        } else {
            free(field->name);
            free(field->type);
            free(field);
            isNull = TRUE;
        }
    }
    metadata->fields = listField;
}

//Libera Metadata
void freeMetadata(Metadata *metadata) {
    free(metadata->fileName);
    free(metadata->keyName);
    free(metadata->keyType);
    for (int i = 0; i < metadata->fieldsLength; i++) {
        free(metadata->fields[i]->name);
        free(metadata->fields[i]->type);
        free(metadata->fields[i]);
    }
    free(metadata->fields);
    free(metadata);
}