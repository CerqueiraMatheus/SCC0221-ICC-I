#ifndef _BITMAP_HELPER_H
#define _BITMAP_HELPER_H

#include <stdint.h>

#include "file_helper.h"

//Tamanho da paleta
#define PALETTE_SIZE 256

#define HEADER_FAIL -1
#define OPEN_FAIL 0
#define SUCCESS 1

//Modos de filtro
#define MODE_NEGATIVE 1
#define MODE_BLACK_WHITE 2

//Finais de nome dos modos de filtro
#define MODE_NEGATIVE_NAME_END "Negativo.bmp"
#define MODE_BLACK_WHITE_NAME_END "PretoBranco.bmp"

//Struct para o cabeçalho de arquivo
typedef struct {
    char signature[2];
    uint32_t fileSize;
    uint32_t reservedField;
    uint32_t offset;
} FileHeader;

//Struct para cabeçalho de BitMap
typedef struct {
    uint32_t headerSize;
    uint32_t imageWidth;
    uint32_t imageHeight;
    uint16_t planeCounter;
    uint16_t bitPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t pixelPerMeterWidth;
    uint32_t pixelPerMeterHeight;
    uint32_t usedColorsCounter;
    uint32_t importantColorsCounter;
} BitMapHeader;

//Struct para cor
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t reserved;
} Color;

//Struct para BitMap
typedef struct {
    FileHeader *fileHeader;
    BitMapHeader *header;
    Color **palette;
    uint8_t **imageData;
} BitMap;

//Funções gerais
int getNextFourMultiple(int baseNumber);
boolean checkFileName(char *fileName);
boolean checkSignature(char *signature);
char *getNewFileName(char *oldName, int mode);

//Funções para exibição
void printBitMapHeaders(BitMap *bmp);
void printBitMapPalette(BitMap *bmp);
void printBitMapDataSum(BitMap *bmp);

//Funções para liberação
void freeBitMap(BitMap **bmp);
void freePalette(Color ***palette);

#endif