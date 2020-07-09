#include <stdio.h>
#include <stdlib.h>

#include "bitmap_helper.h"
#include "bitmap_reader.h"
#include "bitmap_writer.h"

/**
 * Dividi o projeto em 3 seções:
 *  1) Helper (para definições e funções gerais)
 *  2) Reader (para leitura dos arquivos)
 *  3) Writer (para escrita dos arquivos)
 * */

int main() {
    BitMap *originalImage = NULL;
    BitMap *modifiedImage = NULL;
    char *originalFileName = NULL;
    char *modifiedFileName = NULL;
    int mode;

    //Lê os parâmetros
    originalFileName = readLine(stdin);
    scanf("%d", &mode);

    //Caso a leitura não seja igual a SUCCESS,
    //libera ponteiros e cancela a execução
    int result;
    if ((result = readBitMap(originalFileName, &originalImage)) != SUCCESS) {
        //Libera o nome
        free(originalFileName);

        //Se o cabeçalho falhar, a struct precisa ser liberada
        if (result == HEADER_FAIL) {
            free(originalImage->fileHeader);
            free(originalImage);
        }

        //Rompe a execução
        return (EXIT_SUCCESS);
    }

    writeBitMap(originalFileName, originalImage, &modifiedFileName, &modifiedImage, mode);

    //Exibe as informações requisitadas
    printBitMapHeaders(originalImage);
    printf("PALETA ORIGINAL:\n");
    printBitMapPalette(originalImage);
    printf("PALETA NOVA:\n");
    printBitMapPalette(modifiedImage);
    printBitMapDataSum(modifiedImage);
    printf("%s\n", modifiedFileName);

    //Libera a imagem original
    freeBitMap(&originalImage);
    free(originalFileName);

    /**
     * Libera a imagem modificada
     * Obs.: como a imagem nova tem apenas a modificação
     * da paleta, apenas apontei o restante da struct
     * para a original, sendo necessário desalocar apenas
     * a nova paleta. **/
    freePalette(&modifiedImage->palette);
    free(modifiedImage);
    free(modifiedFileName);

    return EXIT_SUCCESS;
}