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
    uint32_t    nPixelPerBit;
    uint32_t    nAlign;
    uint32_t    nNumOfPalette;
    uint32_t    nPadding;
    uint32_t    nSizeOfImage;
    uint32_t    nStride;
    LPRGBQUAD_t pPalette;
    void*       pPixels;
} RGB_INFO_t, *LPRGB_INFO_t;

typedef struct
{
    uint32_t nWidth;
    uint32_t nHeight;
    uint32_t nNumOfElement;
    uint8_t* pYUV;
} YUV_t, *LPYUV_t;

void IMG_RGB_ShowInformation( LPRGB_INFO_t pImageInstance );
void IMG_ConvertDomainRGBtoYUV444( LPRGB_INFO_t pRGBInstance, LPYUV_t pYUVInstance );

#endif  //!__IMAGE__H__
