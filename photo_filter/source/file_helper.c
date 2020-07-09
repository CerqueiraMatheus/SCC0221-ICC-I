#include "file_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap_helper.h"

/**
 * Contém as funções:
 *  print;
 *  free;
 *  uso geral (verificações, leituras e cálculos).
 * 
 * As definições específicas relacionadas a BitMap
 * estão em bitmap_helper.h; as demais em file_helper.h
 * */

//Lê linhas
char *readLine(FILE *stream) {
    char *string = 0;
    int position = 0;

    do {
        if (position % READLINE_BUFFER == 0) {
            string =
                (char *)realloc(string, (position / READLINE_BUFFER + 1) * READLINE_BUFFER);
        }
        char aux = (char)fgetc(stream);

        //Ignora o \r
        if (aux != '\r') string[position++] = aux;

    } while (string[position - 1] != ' ' && string[position - 1] != '\n' && !feof(stream));

    string[position - 1] = '\0';
    string = (char *)realloc(string, position);

    return string;
}

//Retorna o próximo número múltiplo de 4 (caso não for)
int getNextFourMultiple(int number) {
    return (number % 4 != 0) ? (number + 3) / 4 * 4 : number;
}

//Verifica o nome do arquivo (se contém ".bmp")
boolean checkFileName(char *fileName) {
    char *extension = strchr(fileName, '.');
    if (!extension) return FALSE;
    if (strcmp(extension + 1, "bmp")) return FALSE;
    return TRUE;
}

//Checa a assinatura do arquivo (se é igual a "BM")
boolean checkSignature(char *signature) {
    if (signature[0] == 'B')
        if (signature[1] == 'M')
            return TRUE;
    return FALSE;
}

//Gera um novo nome de arquivo a partir de um filtro
char *getNewFileName(char *oldName, int mode) {
    char *newName = NULL;
    switch (mode) {
        case MODE_NEGATIVE:
            newName = (char *)malloc(strlen(oldName) + strlen(MODE_NEGATIVE_NAME_END) - 3);
            strcpy(newName, oldName);
            newName = strtok(newName, ".");
            strcat(newName, MODE_NEGATIVE_NAME_END);
            break;

        case MODE_BLACK_WHITE:
            newName = (char *)malloc(strlen(oldName) + strlen(MODE_BLACK_WHITE_NAME_END) - 3);
            strcpy(newName, oldName);
            newName = strtok(newName, ".");
            strcat(newName, MODE_BLACK_WHITE_NAME_END);
            break;
    }

    return newName;
}

//Imprime o cabeçalho do BitMap
void printBitMapHeaders(BitMap *bmp) {
    printf("CABECALHO:\n");
    printf("Iniciais: %s\n", bmp->fileHeader->signature);
    printf("Tamanho do arquivo: %u\n", bmp->fileHeader->fileSize);
    printf("Reservado: %u\n", bmp->fileHeader->reservedField);
    printf("Deslocamento, em bytes, para o inicio da area de dados: %u\n", bmp->fileHeader->offset);
    printf("Tamanho em bytes do segundo cabecalho: %u\n", bmp->header->headerSize);
    printf("Resolucao: %u x %u\n", bmp->header->imageWidth, bmp->header->imageHeight);
    printf("Numero de planos: %u\n", bmp->header->planeCounter);
    printf("Bits por pixel: %u\n", bmp->header->bitPerPixel);
    printf("Compressao usada: %u\n", bmp->header->compression);
    printf("Tamanho imagem: %u\n", bmp->header->imageSize);
    printf("Resolucao horizontal: %u pixel por metro\n", bmp->header->pixelPerMeterWidth);
    printf("Resolucao Vertical: %u pixel por metro\n", bmp->header->pixelPerMeterHeight);
    printf("Numero de cores usadas: %u\n", bmp->header->usedColorsCounter);
    printf("Numero de cores importantes: %u\n", bmp->header->importantColorsCounter);
}

//Imprime a paleta do BitMap
void printBitMapPalette(BitMap *bmp) {
    for (int i = 0; i < PALETTE_SIZE; i++)
        printf("Paleta[%d]: R:%d G:%d B:%d\n",
               i,
               bmp->palette[i]->red,
               bmp->palette[i]->green,
               bmp->palette[i]->blue);
}

//Imprime a soma das linhas de dados do BitMap
void printBitMapDataSum(BitMap *bmp) {
    long long int sum = 0;

    int width = getNextFourMultiple(bmp->header->imageWidth);

    for (int i = 0; i < bmp->header->imageHeight; sum = 0, i++) {
        for (int j = 0; j < width; j++) {
            if (j < bmp->header->imageWidth) {
                sum += bmp->imageData[i][j];
            } else {
                sum -= 1;
            }
        }
        printf("Soma linha %d: %lld\n", i, sum);
    }
}

//Libera a Paleta
void freePalette(Color ***palette) {
    for (int i = 0; i < PALETTE_SIZE; i++) free((*palette)[i]);
    free(*palette);
}

//Libera o BitMap
void freeBitMap(BitMap **bmp) {
    freePalette(&(*bmp)->palette);
    for (int i = 0; i < (*bmp)->header->imageHeight; i++) free((*bmp)->imageData[i]);
    free((*bmp)->imageData);
    free((*bmp)->fileHeader);
    free((*bmp)->header);
    free(*bmp);
}