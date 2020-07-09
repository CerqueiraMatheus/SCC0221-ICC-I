#include "bitmap_writer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_helper.h"

//Aplica o filtro preto e branco
void doBlackAndWhiteFilter(Color *color) {
    int16_t avg = (color->red + color->green + color->blue) / 3;
    color->blue = avg;
    color->green = avg;
    color->red = avg;
}

//Aplica o filtro negativo
void doNegativeFilter(Color *color) {
    if (color->blue - 255 < 0)
        color->blue = 255 - color->blue;
    else
        color->blue = color->blue - 255;

    if (color->red - 255 < 0)
        color->red = 255 - color->red;
    else
        color->red = color->red - 255;

    if (color->green - 255 < 0)
        color->green = 255 - color->green;
    else
        color->green = color->green - 255;
}

//Aplica um filtro selecionado a todas as cores da paleta
void applyFilterOnPalette(BitMap *bmp, int mode) {
    for (int i = 0; i < PALETTE_SIZE; i++) {
        switch (mode) {
            case MODE_BLACK_WHITE:
                doBlackAndWhiteFilter(bmp->palette[i]);
                break;
            case MODE_NEGATIVE:
                doNegativeFilter(bmp->palette[i]);
                break;
            default:
                break;
        }
    }
}

//Escreve o cabeçalho de arquivo
void writeFileHeader(FILE *imageFile, BitMap *bmp) {
    fwrite(bmp->fileHeader->signature,
           sizeof(bmp->fileHeader->signature),
           1, imageFile);

    fwrite(&bmp->fileHeader->fileSize,
           sizeof(bmp->fileHeader->fileSize),
           1, imageFile);

    fwrite(&bmp->fileHeader->reservedField,
           sizeof(bmp->fileHeader->reservedField),
           1, imageFile);

    fwrite(&bmp->fileHeader->offset,
           sizeof(bmp->fileHeader->offset),
           1, imageFile);
}

//Escreve o cabeçalho de BitMap
void writeBitMapHeader(FILE *imageFile, BitMap *bmp) {
    fwrite(&bmp->header->headerSize,
           sizeof(bmp->header->headerSize),
           1, imageFile);

    fwrite(&bmp->header->imageWidth,
           sizeof(bmp->header->imageWidth),
           1, imageFile);

    fwrite(&bmp->header->imageHeight,
           sizeof(bmp->header->imageHeight),
           1, imageFile);

    fwrite(&bmp->header->planeCounter,
           sizeof(bmp->header->planeCounter),
           1, imageFile);

    fwrite(&bmp->header->bitPerPixel,
           sizeof(bmp->header->bitPerPixel),
           1, imageFile);

    fwrite(&bmp->header->compression,
           sizeof(bmp->header->compression),
           1, imageFile);

    fwrite(&bmp->header->imageSize,
           sizeof(bmp->header->imageSize),
           1, imageFile);

    fwrite(&bmp->header->pixelPerMeterWidth,
           sizeof(bmp->header->pixelPerMeterWidth),
           1, imageFile);

    fwrite(&bmp->header->pixelPerMeterHeight,
           sizeof(bmp->header->pixelPerMeterHeight),
           1, imageFile);

    fwrite(&bmp->header->usedColorsCounter,
           sizeof(bmp->header->usedColorsCounter),
           1, imageFile);

    fwrite(&bmp->header->importantColorsCounter,
           sizeof(bmp->header->importantColorsCounter),
           1, imageFile);
}

//Escreve a paleta de cores
void writeColorPalette(FILE *imageFile, BitMap *bmp) {
    for (int i = 0; i < PALETTE_SIZE; i++) {
        fwrite(&bmp->palette[i]->blue,
               sizeof(bmp->palette[i]->blue),
               1, imageFile);

        fwrite(&bmp->palette[i]->red,
               sizeof(bmp->palette[i]->red),
               1, imageFile);

        fwrite(&bmp->palette[i]->green,
               sizeof(bmp->palette[i]->green),
               1, imageFile);

        fwrite(&bmp->palette[i]->reserved,
               sizeof(bmp->palette[i]->reserved),
               1, imageFile);
    }
}

//Escreve os dados da imagem
void writeImageData(FILE *imageFile, BitMap *bmp) {
    //Percorre os dados de cores
    for (int i = bmp->header->imageHeight - 1; i >= 0; i--) {
        for (int j = 0; j < getNextFourMultiple(bmp->header->imageWidth); j++)
            fwrite(&bmp->imageData[i][j],
                   sizeof(bmp->imageData[i][j]),
                   1, imageFile);
    }
}

//Escreve o BitMap
void writeBitMap(char *originalFileName, BitMap *oldImage, char **modifiedFileName, BitMap **newBitMap, int mode) {
    *newBitMap = (BitMap *)calloc(1, sizeof(BitMap));

    //Atribui os dados do bmp antigo
    (*newBitMap)->fileHeader = oldImage->fileHeader;
    (*newBitMap)->header = oldImage->header;
    (*newBitMap)->imageData = oldImage->imageData;

    //Copia os dados da paleta para serem modificados
    (*newBitMap)->palette = (Color **)calloc(PALETTE_SIZE, sizeof(Color *));
    for (int i = 0; i < PALETTE_SIZE; i++) {
        (*newBitMap)->palette[i] = calloc(1, sizeof(Color));
        (*newBitMap)->palette[i]->blue = oldImage->palette[i]->blue;
        (*newBitMap)->palette[i]->green = oldImage->palette[i]->green;
        (*newBitMap)->palette[i]->red = oldImage->palette[i]->red;
        (*newBitMap)->palette[i]->reserved = oldImage->palette[i]->reserved;
    }

    //Aplica o filtro na paleta
    applyFilterOnPalette(*newBitMap, mode);

    //Gera o novo nome
    (*modifiedFileName) = getNewFileName(originalFileName, mode);

    //Escreve o arquivo
    FILE *imageFile = fopen((*modifiedFileName), "wb");
    writeFileHeader(imageFile, *newBitMap);
    writeBitMapHeader(imageFile, *newBitMap);
    writeColorPalette(imageFile, *newBitMap);
    writeImageData(imageFile, *newBitMap);

    //Fecha o arquivo
    fclose(imageFile);
}