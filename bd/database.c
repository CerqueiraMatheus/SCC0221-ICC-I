#include <stdio.h>
#include <stdlib.h>

#include "boolean.h"
#include "extractor.h"
#include "index.h"
#include "metadata.h"
#include "register.h"

//Limpa stdin
void cleanInput() {
    int c;
    //Consome todos os caracteres de stdin
    while (((c = fgetc(stdin)) != '\n') && (c != EOF));
}

int main() {
    char *metadataFileName = 0;
    int opt = 0;
    int index;
    boolean indexCreated = FALSE;

    metadataFileName = getLineFromFile(stdin);

    Metadata *metadata = NULL;
    metadata = readMetaData(metadataFileName);

    Register *reg = NULL;
    int registerSize = 0;

    do {
        opt = getOptionFromFile(stdin);

        switch (opt) {
            case INSERT:
                readRegisterFromString(&reg, getLineFromFile(stdin), metadata);
                writeRegisterToFile(reg, metadata);
                freeRegister(&reg, metadata);
                break;

            case INDEX:
                writeIndex(metadata);
                indexCreated = TRUE;
                break;

            case SEARCH:
                if (!indexCreated) writeIndex(metadata);
                scanf(" %d", &index);
                cleanInput();
                if (readRegisterFromFile(metadata, &reg, index)) {
                    printRegister(reg, metadata);
                    freeRegister(&reg, metadata);
                }
                break;

            default:
                break;
        }
    } while (opt != EXIT);

    freeMetadata(metadata);
    free(metadataFileName);

    return EXIT_SUCCESS;
}