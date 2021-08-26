#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Bitmap.h"

#define BIT2BYTE(VAL)   ( (VAL) / 8U )

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

}

void BMP_ShowInformation( LPBMP_t pBMPInstance )
{

}