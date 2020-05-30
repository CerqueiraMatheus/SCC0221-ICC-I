#include <file_extractor.h>
#include <metadata.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *fileName = 0;

    fileName = getLineFromFile(stdin);
    printf("%s\n", fileName);

    Metadata *metadata = NULL;
    metadata = readMetaData(fileName);
    printf("%s\n", metadata->fileName);

    freeMetadata(metadata);
    free(fileName);

    return EXIT_SUCCESS;
}