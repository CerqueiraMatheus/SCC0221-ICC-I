#include "bitmap_reader.h"

#include <stdio.h>
#include <stdlib.h>

#include "file_helper.h"

//Lê o cabeçalho de arquivo da imagem
void readFileHeader(FILE *imageFile, BitMap *bmp) {
    //Aloca o cabeçalho de arquivo
    bmp->fileHeader = (FileHeader *)calloc(1, sizeof(FileHeader));

    fread(bmp->fileHeader->signature,
          sizeof(bmp->fileHeader->signature),
          1, imageFile);

    fread(&bmp->fileHeader->fileSize,
          sizeof(bmp->fileHeader->fileSize),
          1, imageFile);

    fread(&bmp->fileHeader->reservedField,
          sizeof(bmp->fileHeader->reservedField),
          1, imageFile);

    fread(&bmp->fileHeader->offset,
          sizeof(bmp->fileHeader->offset),
          1, imageFile);
}

//Lê o cabeçalho de BitMap da imagem
void readBitMapHeader(FILE *imageFile, BitMap *bmp) {
    //Aloca o cabeçalho de BitMap
    bmp->header = (BitMapHeader *)calloc(1, sizeof(BitMapHeader));

    //Lê as informações do cabeçalho do BitMap
    fread(&bmp->header->headerSize,
          sizeof(bmp->header->headerSize),
          1, imageFile);

    fread(&bmp->header->imageWidth,
          sizeof(bmp->header->imageWidth),
          1, imageFile);

    fread(&bmp->header->imageHeight,
          sizeof(bmp->header->imageHeight),
          1, imageFile);

    fread(&bmp->header->planeCounter,
          sizeof(bmp->header->planeCounter),
          1, imageFile);

    fread(&bmp->header->bitPerPixel,
          sizeof(bmp->header->bitPerPixel),
          1, imageFile);

    fread(&bmp->header->compression,
          sizeof(bmp->header->compression),
          1, imageFile);

    fread(&bmp->header->imageSize,
          sizeof(bmp->header->imageSize),
          1, imageFile);

    fread(&bmp->header->pixelPerMeterWidth,
          sizeof(bmp->header->pixelPerMeterWidth),
          1, imageFile);

    fread(&bmp->header->pixelPerMeterHeight,
          sizeof(bmp->header->pixelPerMeterHeight),
          1, imageFile);

    fread(&bmp->header->usedColorsCounter,
          sizeof(bmp->header->usedColorsCounter),
          1, imageFile);

    fread(&bmp->header->importantColorsCounter,
          sizeof(bmp->header->importantColorsCounter),
          1, imageFile);
}

//Lê a paleta de cores da imagem
void readColorPalette(FILE *imageFile, BitMap *bmp) {
    //Aloca o array de ponteiros de Color
    bmp->palette = (Color **)calloc(PALETTE_SIZE, sizeof(Color *));

    //Percorre a paleta
    for (int i = 0; i < PALETTE_SIZE; i++) {
        //Aloca o ponteiro de Color
        bmp->palette[i] = (Color *)calloc(1, sizeof(Color));

        //Recebe as informações de Color
        fread(&bmp->palette[i]->blue,
              sizeof(bmp->palette[i]->blue),
              1, imageFile);

        fread(&bmp->palette[i]->green,
              sizeof(bmp->palette[i]->green),
              1, imageFile);

        fread(&bmp->palette[i]->red,
              sizeof(bmp->palette[i]->red),
              1, imageFile);

        fread(&bmp->palette[i]->reserved,
              sizeof(bmp->palette[i]->reserved),
              1, imageFile);
    }
}

//Lê os dados da imagem
void readImageData(FILE *imageFile, BitMap *bmp) {
    //Aloca o array de índices de Color
    bmp->imageData = (uint8_t **)calloc(bmp->header->imageHeight, sizeof(uint8_t *));

    //Atribui a largura considerando os "lixos"
    int width = getNextFourMultiple(bmp->header->imageWidth);

    //Percorre os dados de cores
    for (int i = bmp->header->imageHeight - 1; i >= 0; i--) {
        bmp->imageData[i] = (uint8_t *)calloc(width, sizeof(uint8_t));
        for (int j = 0; j < width; j++) {
            fread(&bmp->imageData[i][j],
                  sizeof(uint8_t),
                  1, imageFile);
        }
    }
}

//Lê o BitMap
int readBitMap(char *imageName, BitMap **bmp) {
    FILE *imageFile = NULL;

    //Validação do nome do arquivo
    if (!checkFileName(imageName)) {
        printf("Arquivo nao eh do formato BMP\n");
        return OPEN_FAIL;
    }
    //Validação de abertura do arquivo
    if (!(imageFile = fopen(imageName, "rb"))) {
        printf("Erro no arquivo\n");
        return OPEN_FAIL;
    }

    //Aloca memória para o BitMap
    *bmp = (BitMap *)calloc(1, sizeof(BitMap));

    //Lê o cabeçalho do arquivo
    readFileHeader(imageFile, *bmp);

    //Validação da assinatura do arquivo
    if (!checkSignature((*bmp)->fileHeader->signature)) {
        printf("Arquivo nao eh do formato BMP\n");
        return HEADER_FAIL;
    }

    //Lê os demais componentes
    readBitMapHeader(imageFile, *bmp);
    readColorPalette(imageFile, *bmp);
    readImageData(imageFile, *bmp);

    //Fecha o arquivo
    fclose(imageFile);

    return SUCCESS;
}