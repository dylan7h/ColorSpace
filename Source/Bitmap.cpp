#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Bitmap.h"

#define BIT2BYTE(VAL)   ( (VAL) / 8U )

#define RESULTION_32BIT     ( 4U )
#define RESULTION_24BIT     ( 3U )
#define RESULTION_16BIT     ( 2U )
#define RESULTION_8BIT      ( 1U )
#define RESULTION_TARGET    RESULTION_24BIT

void BMP_LoadFromFile( const char lpszPath[], LPBMP_t pBMPInstance )
{
    FILE* fp;
    uint32_t SizeOfPalette, NumOfBlock;

    assert( pBMPInstance != nullptr );

    /* Open Bitmap File */
    fp = fopen( lpszPath, "rb" );
    assert( fp != nullptr );

    /* Read BMP_FILE_t */
    assert( fread( (void*)&pBMPInstance->File, 1, SIZE_OF_BMP_FILE_HEADER, fp ) > 0 );
    assert( pBMPInstance->File.bfType == BMP_SIGNATURE );

    /* Read BMP_INFO_t */
    assert( fread( (void*)&pBMPInstance->Info, 1, SIZE_OF_BMP_INFO_HEADER, fp ) > 0 );
    assert( pBMPInstance->Info.biSize == SIZE_OF_BMP_INFO_HEADER );

    /* Set Basic Data Of Image Stucture */
    pBMPInstance->Image.nWidth          = pBMPInstance->Info.biWidth;
    pBMPInstance->Image.nHeight         = pBMPInstance->Info.biHeight;
    pBMPInstance->Image.nPixelPerBit    = pBMPInstance->Info.biBitCount;
    pBMPInstance->Image.nSizeOfImage    = pBMPInstance->Info.biSizeImage;

    /* Set Align Value */
    switch ( pBMPInstance->Image.nPixelPerBit )
    {
        case 1U:
            pBMPInstance->Image.nAlign = 32U;
        break;

        case 4U:
            pBMPInstance->Image.nAlign = 8U;
        break;
    
        default:
            pBMPInstance->Image.nAlign = 4U;
        break;
    }

    /* Caculate Size Of Padding */
    pBMPInstance->Image.nPadding = pBMPInstance->Image.nAlign - ( pBMPInstance->Image.nWidth % pBMPInstance->Image.nAlign );
    if( pBMPInstance->Image.nPadding == pBMPInstance->Image.nAlign )
    {
        pBMPInstance->Image.nPadding = 0U;
    }

    /* Calculate Stride Of Image */
    pBMPInstance->Image.nStride = BIT2BYTE( ( pBMPInstance->Image.nWidth + pBMPInstance->Image.nPadding ) * pBMPInstance->Image.nPixelPerBit );

    /* Load Color Palette if exist */
    SizeOfPalette = pBMPInstance->File.bfOffBits - SIZE_OF_BMP_HEADER;
    if( SizeOfPalette > 0U )
    {
        pBMPInstance->Image.nNumOfPalette = SizeOfPalette / sizeof( RGBQUAD_t );
        pBMPInstance->Image.pPalette = (LPRGBQUAD_t)malloc( SizeOfPalette );
        assert( pBMPInstance->Image.pPalette != nullptr );

        assert( fread( pBMPInstance->Image.pPalette, sizeof( RGBQUAD_t ), pBMPInstance->Image.nNumOfPalette, fp ) == pBMPInstance->Image.nNumOfPalette );
    }
    else
    {
        pBMPInstance->Image.pPalette        = nullptr;
        pBMPInstance->Image.nNumOfPalette   = 0U;
    }

    /* Load Pixel Data */
    pBMPInstance->Image.pPixels = malloc( pBMPInstance->Image.nSizeOfImage );
    assert( pBMPInstance->Image.pPixels != nullptr );

    NumOfBlock = pBMPInstance->Image.nSizeOfImage / pBMPInstance->Image.nAlign;
    assert( fread( pBMPInstance->Image.pPixels, pBMPInstance->Image.nAlign, NumOfBlock, fp ) == NumOfBlock );

    (void)fclose( fp );
}

void BMP_ApplyPalette( LPBMP_t pDstBMPInstance, LPBMP_t pSrcBMPInstance )
{
    uint8_t* pDstPos;
    uint8_t* pSrcPos;
    uint32_t nPalettePos, nNumOfPixels;

    assert( pSrcBMPInstance->Image.nPixelPerBit <= 8U );

    pDstBMPInstance->File = pSrcBMPInstance->File;
    pDstBMPInstance->Info = pSrcBMPInstance->Info;
    pDstBMPInstance->Image = pSrcBMPInstance->Image;

    nNumOfPixels = pSrcBMPInstance->Image.nSizeOfImage * ( 8U / pSrcBMPInstance->Image.nPixelPerBit );
    pDstBMPInstance->Image.pPixels = malloc( RESULTION_TARGET * nNumOfPixels );
    assert( pDstBMPInstance->Image.pPixels != nullptr );

    pDstPos = (uint8_t*)pDstBMPInstance->Image.pPixels;
    pSrcPos = (uint8_t*)pSrcBMPInstance->Image.pPixels;
    for( uint32_t i = 0; i < nNumOfPixels; i++ )
    {
        switch ( pSrcBMPInstance->Image.nPixelPerBit )
        {
            case 1U:
                nPalettePos = ( pSrcPos[ i / 8U ] >> ( i % 8U ) ) & 0x01U;
            break;

            case 2U:
                nPalettePos = ( pSrcPos[ i / 4U ] >> ( ( i % 4U ) * 2U ) ) & 0x03U;
            break;

            case 4U:
                nPalettePos = ( pSrcPos[ i / 2U ] >> ( ( i % 2U ) * 4U ) ) & 0x0FU;
            break;

            default:
                nPalettePos = pSrcPos[ i ];
            break;
        }

        pDstPos[ ( RESULTION_TARGET * i ) ]         = pSrcBMPInstance->Image.pPalette[ nPalettePos ].rgbR;
        pDstPos[ ( RESULTION_TARGET * i ) + 1U ]    = pSrcBMPInstance->Image.pPalette[ nPalettePos ].rgbG;
        pDstPos[ ( RESULTION_TARGET * i ) + 2U ]    = pSrcBMPInstance->Image.pPalette[ nPalettePos ].rgbB;
    }

    pDstBMPInstance->File.bfSize            = SIZE_OF_BMP_HEADER + ( RESULTION_TARGET * pSrcBMPInstance->Image.nSizeOfImage );
    pDstBMPInstance->File.bfOffBits         = SIZE_OF_BMP_HEADER;

    pDstBMPInstance->Info.biBitCount        = 24U;
    pDstBMPInstance->Info.biSizeImage       = ( RESULTION_TARGET * pSrcBMPInstance->Image.nSizeOfImage );

    pDstBMPInstance->Image.nPixelPerBit     = 24U;
    pDstBMPInstance->Image.nAlign           = 4U;
    pDstBMPInstance->Image.nNumOfPalette    = 0U;
    pDstBMPInstance->Image.nSizeOfImage     = ( RESULTION_TARGET * pSrcBMPInstance->Image.nSizeOfImage );
    pDstBMPInstance->Image.pPalette         = nullptr;
    pDstBMPInstance->Image.nPadding         = pDstBMPInstance->Image.nAlign - ( pDstBMPInstance->Image.nWidth % pDstBMPInstance->Image.nAlign );
    if( pDstBMPInstance->Image.nPadding == pDstBMPInstance->Image.nAlign )
    {
        pDstBMPInstance->Image.nPadding = 0U;
    }

    pDstBMPInstance->Image.nStride = BIT2BYTE( ( pDstBMPInstance->Image.nWidth + pDstBMPInstance->Image.nPadding ) * pDstBMPInstance->Image.nPixelPerBit );
}

void BMP_ShowInformation( LPBMP_t pBMPInstance )
{
    printf( "[ BMP_FILE_t ]\n" );
    printf( "    - bfType           : 0x%02X( %c%c )\n", pBMPInstance->File.bfType, pBMPInstance->File.bfType & 0xFF, pBMPInstance->File.bfType >> 8 );
    printf( "    - bfRes1           : 0x%02X\n", pBMPInstance->File.bfReserved1 );
    printf( "    - bfRes2           : 0x%02X\n", pBMPInstance->File.bfReserved2 );
    printf( "    - bfOffBits        : %u\n\n", pBMPInstance->File.bfOffBits );

    printf( "[ BMP_FILE_t ]\n" );
    printf( "    - biSize           : %u\n", pBMPInstance->Info.biSize );
    printf( "    - biWidth          : %u\n", pBMPInstance->Info.biWidth );
    printf( "    - biHeight         : %u\n", pBMPInstance->Info.biHeight );
    printf( "    - biPlanes         : %u\n", pBMPInstance->Info.biPlanes );
    printf( "    - biBitCount       : %u\n", pBMPInstance->Info.biBitCount );
    printf( "    - biCompression    : %u\n", pBMPInstance->Info.biCompression );
    printf( "    - biSizeImage      : %u\n", pBMPInstance->Info.biSizeImage );
    printf( "    - biXPerlsPerMeter : %u\n", pBMPInstance->Info.biXPerlsPerMeter );
    printf( "    - biYPerlsPerMeter : %u\n", pBMPInstance->Info.biYPerlsPerMeter );
    printf( "    - biClrUsed        : %u\n", pBMPInstance->Info.biClrUsed );
    printf( "    - biClrImportant   : %u\n\n", pBMPInstance->Info.biClrImportant );
}