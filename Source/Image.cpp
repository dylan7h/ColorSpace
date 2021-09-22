#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Image.h"

#define RGB_B_POS               (0)
#define RGB_G_POS               (1)
#define RGB_R_POS               (2)
#define RGB_DELI                (3)

#define YUV444_Y_POS            (0)
#define YUV444_U_POS            (1)
#define YUV444_V_POS            (2)
#define YUV444_DELI             (3)

#define CLIP(X)                 ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B)          CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B)          CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B)          CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

// YUV -> RGB
#define C(Y)                    ( (Y) - 16  )
#define D(U)                    ( (U) - 128 )
#define E(V)                    ( (V) - 128 )

#define YUV2R(Y, U, V)          CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V)          CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V)          CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)

// RGB -> YCbCr
#define CRGB2Y(R, G, B)         CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16)
#define CRGB2Cb(R, G, B)        CLIP((36962 * (B - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)
#define CRGB2Cr(R, G, B)        CLIP((46727 * (R - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)

// YCbCr -> RGB
#define CYCbCr2R(Y, Cb, Cr)     CLIP( Y + ( 91881 * Cr >> 16 ) - 179 )
#define CYCbCr2G(Y, Cb, Cr)     CLIP( Y - (( 22544 * Cb + 46793 * Cr ) >> 16) + 135)
#define CYCbCr2B(Y, Cb, Cr)     CLIP( Y + (116129 * Cb >> 16 ) - 226 )


void IMG_RGB_ShowInformation( LPRGB_INFO_t pImageInstance )
{
    printf( "[ IMG_INFO ]\n" );
    printf( "    - nWidth           : %u\n", pImageInstance->nWidth );
    printf( "    - nHeight          : %u\n", pImageInstance->nHeight );
    printf( "    - nPixelPerBit     : %u\n", pImageInstance->nPixelPerBit );
    printf( "    - nNumOfPalette    : %u\n", pImageInstance->nNumOfPalette );
    printf( "    - nPadding         : %u\n", pImageInstance->nPadding );
    printf( "    - nSizeOfImage     : %u\n", pImageInstance->nSizeOfImage );
    printf( "    - nStride          : %u\n\n", pImageInstance->nStride );
}

void IMG_ConvertDomainRGBtoYUV( LPRGB_INFO_t pRGBInstance, LPYUV_t pYUVInstance )
{
    volatile uint32_t i;
    volatile int32_t rgbR;
    volatile int32_t rgbG;
    volatile int32_t rgbB;
    uint8_t* pRGBPos;

    assert( pRGBInstance != nullptr );
    assert( pYUVInstance != nullptr );

    pYUVInstance->nWidth = pRGBInstance->nWidth;
    pYUVInstance->nHeight = pRGBInstance->nHeight;
    pYUVInstance->nNumOfElement = pYUVInstance->nWidth * pYUVInstance->nHeight;

    pYUVInstance->pYBuffer = (uint8_t*)malloc( pYUVInstance->nNumOfElement );
    pYUVInstance->pUBuffer = (uint8_t*)malloc( pYUVInstance->nNumOfElement );
    pYUVInstance->pVBuffer = (uint8_t*)malloc( pYUVInstance->nNumOfElement );
    assert( pYUVInstance->pYBuffer != nullptr );
    assert( pYUVInstance->pUBuffer != nullptr );
    assert( pYUVInstance->pVBuffer != nullptr );

    pRGBPos = (uint8_t*)pRGBInstance->pPixels;
    for( i = 0; i < pYUVInstance->nNumOfElement; i++ )
    {
        rgbB = pRGBPos[ ( i * RGB_DELI ) + RGB_B_POS ];
        rgbG = pRGBPos[ ( i * RGB_DELI ) + RGB_G_POS ];
        rgbR = pRGBPos[ ( i * RGB_DELI ) + RGB_R_POS ];

        /* https://docs.microsoft.com/ko-kr/windows/win32/medfound/recommended-8-bit-yuv-formats-for-video-rendering?redirectedfrom=MSDN#YUV422formats16bitsperpixel */
        pYUVInstance->pYBuffer[ i ] = (uint8_t)( (  66 * rgbR + 129 * rgbG +  25 * rgbB + 128) >> 8) +  16;
        pYUVInstance->pUBuffer[ i ] = (uint8_t)( ( -38 * rgbR -  74 * rgbG + 112 * rgbB + 128) >> 8) + 128;
        pYUVInstance->pVBuffer[ i ] = (uint8_t)( ( 112 * rgbR -  94 * rgbG -  18 * rgbB + 128) >> 8) + 128;
    }
}

void IMG_ConvertYUVtoYUV444( LPYUV_t pYUVInstance )
{
    volatile uint32_t i;
    uint8_t* pYUVPos;

    pYUVInstance->pYUV = (uint8_t*)malloc( pYUVInstance->nNumOfElement * YUV444_DELI );
    assert( pYUVInstance->pYUV != nullptr );

    pYUVPos = pYUVInstance->pYUV;
    for( i = 0; i < pYUVInstance->nNumOfElement; i++ )
    {
        *pYUVPos++ = pYUVInstance->pYBuffer[ i ];
    }

    for( i = 0; i < pYUVInstance->nNumOfElement; i++ )
    {
        *pYUVPos++ = pYUVInstance->pUBuffer[ i ];
    }

    for( i = 0; i < pYUVInstance->nNumOfElement; i++ )
    {
        *pYUVPos++ = pYUVInstance->pVBuffer[ i ];
    }

    FILE* fp;
    char path[ 255 ] = IMG_DIR;
    strcat(path, "/IMG_YUV444.yuv");

    printf( "%s\n", path );

    fp = fopen(path, "wb");
    assert( fp != nullptr );
    fwrite( pYUVInstance->pYUV, pYUVInstance->nNumOfElement, YUV444_DELI, fp );
    fclose( fp );
}

void IMG_ConvertYUVtoYUYV( LPYUV_t pYUVInstance )
{
    volatile uint32_t i;
    uint8_t* pYUVPos;
    uint8_t* pYPos;
    uint8_t* pUPos;
    uint8_t* pVPos;
    uint32_t SizeOfImage;

    SizeOfImage = pYUVInstance->nNumOfElement + ( pYUVInstance->nNumOfElement / 2 ) + ( pYUVInstance->nNumOfElement / 2 );
    pYUVInstance->pYUV = (uint8_t*)malloc( SizeOfImage );
    assert( pYUVInstance->pYUV != nullptr );

    pYUVPos = pYUVInstance->pYUV;
    pYPos = pYUVInstance->pYBuffer;
    pUPos = pYUVInstance->pUBuffer;
    pVPos = pYUVInstance->pVBuffer;
    for(i = 0; i < pYUVInstance->nNumOfElement / 2; i++ )
    {
        *pYUVPos++ = *pYPos++;
        *pYUVPos++ = *pUPos++;  pUPos++;
        *pYUVPos++ = *pYPos++;
        *pYUVPos++ = *pVPos++;  pVPos++;
    }

    FILE* fp;
    char path[ 255 ] = IMG_DIR;
    strcat(path, "/IMG_YUYV.yuv");

    printf( "%s\n", path );

    fp = fopen(path, "wb");
    assert( fp != nullptr );
    fwrite( pYUVInstance->pYUV, 1, SizeOfImage, fp );
    fclose( fp );
}

void IMG_ConvertYUVtoUYVY( LPYUV_t pYUVInstance )
{
    volatile uint32_t i;
    uint8_t* pYUVPos;
    uint8_t* pYPos;
    uint8_t* pUPos;
    uint8_t* pVPos;
    uint32_t SizeOfImage;

    SizeOfImage = pYUVInstance->nNumOfElement + ( pYUVInstance->nNumOfElement / 2 ) + ( pYUVInstance->nNumOfElement / 2 );
    pYUVInstance->pYUV = (uint8_t*)malloc( SizeOfImage );
    assert( pYUVInstance->pYUV != nullptr );

    pYUVPos = pYUVInstance->pYUV;
    pYPos = pYUVInstance->pYBuffer;
    pUPos = pYUVInstance->pUBuffer;
    pVPos = pYUVInstance->pVBuffer;
    for(i = 0; i < pYUVInstance->nNumOfElement / 2; i++ )
    {
        *pYUVPos++ = *pUPos++;  pUPos++;
        *pYUVPos++ = *pYPos++;
        *pYUVPos++ = *pVPos++;  pVPos++;
        *pYUVPos++ = *pYPos++;
    }

    FILE* fp;
    char path[ 255 ] = IMG_DIR;
    strcat(path, "/IMG_UYVY.yuv");

    printf( "%s\n", path );

    fp = fopen(path, "wb");
    assert( fp != nullptr );
    fwrite( pYUVInstance->pYUV, 1, SizeOfImage, fp );
    fclose( fp );
}

void IMG_ConvertYUVtoNV12( LPYUV_t pYUVInstance )
{
    volatile uint32_t i;
    uint8_t* pYUVPos;
    uint8_t* pYPos;
    uint8_t* pUPos;
    uint8_t* pVPos;
    uint32_t SizeOfImage;

    SizeOfImage = pYUVInstance->nNumOfElement + ( pYUVInstance->nNumOfElement / 4 ) + ( pYUVInstance->nNumOfElement / 4 );
    pYUVInstance->pYUV = (uint8_t*)malloc( SizeOfImage );
    assert( pYUVInstance->pYUV != nullptr );

    pYUVPos = pYUVInstance->pYUV;
    pYPos = pYUVInstance->pYBuffer;
    pUPos = pYUVInstance->pUBuffer;
    pVPos = pYUVInstance->pVBuffer;
    for(i = 0; i < pYUVInstance->nNumOfElement; i++ )
    {
        *pYUVPos++ = *pYPos++;
    }

    for(i = 0; i < pYUVInstance->nNumOfElement / 4; i++ )
    {
        *pYUVPos++ = *pUPos++;  pUPos++;
        *pYUVPos++ = *pVPos++;  pVPos++;
    }
    

    FILE* fp;
    char path[ 255 ] = IMG_DIR;
    strcat(path, "/IMG_NV12.yuv");

    printf( "%s\n", path );

    fp = fopen(path, "wb");
    assert( fp != nullptr );
    fwrite( pYUVInstance->pYUV, 1, SizeOfImage, fp );
    fclose( fp );
}

void IMG_ConvertYUVtoNV21( LPYUV_t pYUVInstance )
{
    volatile uint32_t i;
    uint8_t* pYUVPos;
    uint8_t* pYPos;
    uint8_t* pUPos;
    uint8_t* pVPos;
    uint32_t SizeOfImage;

    SizeOfImage = pYUVInstance->nNumOfElement + ( pYUVInstance->nNumOfElement / 4 ) + ( pYUVInstance->nNumOfElement / 4 );
    pYUVInstance->pYUV = (uint8_t*)malloc( SizeOfImage );
    assert( pYUVInstance->pYUV != nullptr );

    pYUVPos = pYUVInstance->pYUV;
    pYPos = pYUVInstance->pYBuffer;
    pUPos = pYUVInstance->pUBuffer;
    pVPos = pYUVInstance->pVBuffer;
    for(i = 0; i < pYUVInstance->nNumOfElement; i++ )
    {
        *pYUVPos++ = *pYPos++;
    }

    for(i = 0; i < pYUVInstance->nNumOfElement / 4; i++ )
    {
        *pYUVPos++ = *pVPos++;  pVPos++;
        *pYUVPos++ = *pUPos++;  pUPos++;
    }

    FILE* fp;
    char path[ 255 ] = IMG_DIR;
    strcat(path, "/IMG_NV21.yuv");

    printf( "%s\n", path );

    fp = fopen(path, "wb");
    assert( fp != nullptr );
    fwrite( pYUVInstance->pYUV, 1, SizeOfImage, fp );
    fclose( fp );
}

