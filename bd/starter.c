#include <stdio.h>
#include <stdlib.h>

#include "extractor.h"
#include "metadata.h"
#include "register.h"

int main() {
    char *fileName = 0;
    int opt = 0;

    fileName = getLineFromFile(stdin);
    printf("%s\n", fileName);

    Metadata *metadata = NULL;
    metadata = readMetaData(fileName);

    Register *reg = NULL;
    int registerSize = 0;

    do {
        opt = getOptFromFile(stdin);

        switch (opt) {
            case INSERT:
                readRegisterFromString(&reg, getLineFromFile(stdin), metadata);
                writeRegister(reg, metadata);
                freeRegister(&reg);
                break;

            case INDEX:
                break;

            case SEARCH:
                break;

            default:
                break;
        }
    } while (opt != EXIT);

    freeMetadata(metadata);
    free(fileName);

    return EXIT_SUCCESS;
}