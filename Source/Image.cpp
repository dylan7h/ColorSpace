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
#define YUV444_DELI             (4)

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

void IMG_ConvertDomainRGBtoYUV444( LPRGB_INFO_t pRGBInstance, LPYUV_t pYUVInstance )
{
    uint32_t i;
    uint8_t* pRGBPos;
    uint8_t rgb[ RGB_DELI ];
    volatile int32_t val;

    assert( pRGBInstance != nullptr );
    assert( pYUVInstance != nullptr );

    pYUVInstance->nWidth = pRGBInstance->nWidth;
    pYUVInstance->nHeight = pRGBInstance->nHeight;
    pYUVInstance->nNumOfElement = pYUVInstance->nWidth * pYUVInstance->nHeight;

    pYUVInstance->pYUV = (uint8_t*)malloc( pYUVInstance->nNumOfElement * YUV444_DELI );
    assert( pYUVInstance->pYUV != nullptr );
    memset( pYUVInstance->pYUV, 0, pYUVInstance->nNumOfElement * YUV444_DELI );

    pRGBPos = (uint8_t*)pRGBInstance->pPixels;
    for( i = 0; i < pYUVInstance->nNumOfElement; i++ )
    {
        rgb[ RGB_B_POS ] = pRGBPos[ ( i * RGB_DELI ) + RGB_B_POS ];
        rgb[ RGB_G_POS ] = pRGBPos[ ( i * RGB_DELI ) + RGB_G_POS ];
        rgb[ RGB_R_POS ] = pRGBPos[ ( i * RGB_DELI ) + RGB_R_POS ];

#if 0
        pYUVInstance->pYUV[ ( i * YUV444_DELI ) + YUV444_Y_POS ] = RGB2Y( rgb[ RGB_R_POS ], rgb[ RGB_G_POS ], rgb[ RGB_B_POS ] );
        pYUVInstance->pYUV[ ( i * YUV444_DELI ) + YUV444_V_POS ] = RGB2V( rgb[ RGB_R_POS ], rgb[ RGB_G_POS ], rgb[ RGB_B_POS ] );
        pYUVInstance->pYUV[ ( i * YUV444_DELI ) + YUV444_U_POS ] = RGB2U( rgb[ RGB_R_POS ], rgb[ RGB_G_POS ], rgb[ RGB_B_POS ] );
#else
        val = ( (  66 * rgb[ RGB_R_POS ] + 129 * rgb[ RGB_G_POS ] +  25 * rgb[ RGB_B_POS ] + 128) >> 8) +  16;
        pYUVInstance->pYUV[ ( i * YUV444_DELI ) + YUV444_Y_POS ] = (uint8_t)val;

        val = ( ( -38 * rgb[ RGB_R_POS ] -  74 * rgb[ RGB_G_POS ] + 112 * rgb[ RGB_B_POS ] + 128) >> 8) + 128;
        pYUVInstance->pYUV[ ( i * YUV444_DELI ) + YUV444_U_POS ] = (uint8_t)val;

        val = ( ( 112 * rgb[ RGB_R_POS ] -  94 * rgb[ RGB_G_POS ] -  18 * rgb[ RGB_B_POS ] + 128) >> 8) + 128;
        pYUVInstance->pYUV[ ( i * YUV444_DELI ) + YUV444_V_POS ] = (uint8_t)val;
#endif
    }

    FILE* fp;
    char path[ 255 ] = IMG_DIR;
    strcat(path, "/IMG_YUV444.yuv");

    printf( "%s\n", path );

    fp = fopen(path, "wb");
    assert( fp != nullptr );
    fwrite( pYUVInstance->pYUV, 1, pYUVInstance->nNumOfElement * YUV444_DELI, fp );
    fclose( fp );
}
