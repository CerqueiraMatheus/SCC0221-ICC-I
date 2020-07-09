#ifndef _BITMAP_WRITER_H
#define _BITMAP_WRITER_H

#include "bitmap_helper.h"
#include "file_helper.h"

void writeBitMap(char *originalFileName,
                 BitMap *oldImage,
                 char **modifiedFileName,
                 BitMap **newBitMap, int mode);

#endif