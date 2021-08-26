#ifndef __IMAGE__H__
#define __IMAGE__H__

#include <stdint.h>

typedef struct
{
    uint8_t rgbR;
    uint8_t rgbG;
    uint8_t rgbB;
    uint8_t rgbA;
} RGBQUAD_t, *LPRGBQUAD_t;

typedef struct
{
    uint32_t    nWidth;
    uint32_t    nHeight;
    uint16_t    nPixelPerBit;
    uint16_t    nAlign;
    uint16_t    nNumOfPalette;
    uint16_t    nPadding;
    uint32_t    nSizeOfImage;
    uint32_t    nStride;
    LPRGBQUAD_t pPalette;
    void*       pPixels;
} IMG_INFO_t;

#endif  //!__IMAGE__H__
