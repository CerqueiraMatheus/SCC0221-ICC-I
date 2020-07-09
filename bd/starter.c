#include <extractor.h>
#include <metadata.h>
#include <register.h>
#include <stdio.h>
#include <stdlib.h>

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
        printf("OPT::: %d\n", opt);

        switch (opt) {
            case INSERT:
                reg = readRegister(getLineFromFile(stdin), metadata);
                registerSize = writeRegister(reg, metadata);
                printf("REGSIZE::: %d\n", registerSize);
                freeRegister(reg);
                break;

            case INDEX:
                break;

            case SEARCH:
                break;
        }
    } while (opt != EXIT);

    freeMetadata(metadata);
    free(fileName);

    return EXIT_SUCCESS;
}