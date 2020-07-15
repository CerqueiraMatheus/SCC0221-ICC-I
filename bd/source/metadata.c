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

//Lê o cabeçalho de Metadata
void readHeader(FILE *metadataFile, Metadata *metadata) {
    metadata->fileName = findValueFromName(getLineFromFile(metadataFile), FILENAME);
    metadata->keyName = findValueFromName(getLineFromFile(metadataFile), KEYNAME);
    metadata->keyType = findValueFromName(getLineFromFile(metadataFile), KEYTYPE);
}

//Lê o corpo de Metadata
void readBody(FILE *metadataFile, Metadata *metadata) {
    boolean isNull = FALSE;
    metadata->fieldCounter = 0;

    //Percorre os campos
    Field **listField = NULL;
    for (int i = 0; !isNull; i++) {
        //Inicializa o campo
        listField = (Field **)realloc(listField, sizeof(Field *) * (i + 1));
        Field *field = (Field *)malloc(sizeof(Field));

        //Recebe os campos
        field->name = findValueFromName(getLineFromFile(metadataFile), FIELDNAME);
        field->type = findValueFromName(getLineFromFile(metadataFile), FIELDTYPE);

        //Enquanto houver campo em nome e tipo armazena
        //o campo
        if (strlen(field->name) && strlen(field->type)) {
            listField[i] = field;
            metadata->fieldCounter++;
        } else {
            //Libera os campos obtidos
            //e ativa o terminador
            free(field->name);
            free(field->type);
            free(field);
            isNull = TRUE;
        }
    }
    //Seta os campos obtidos em Metadata
    metadata->fields = listField;
}

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

//Libera Metadata
void freeMetadata(Metadata *metadata) {
    free(metadata->fileName);
    free(metadata->keyName);
    free(metadata->keyType);
    for (int i = 0; i < metadata->fieldCounter; i++) {
        free(metadata->fields[i]->name);
        free(metadata->fields[i]->type);
        free(metadata->fields[i]);
    }
    free(metadata->fields);
    free(metadata);
}