#include <stdio.h>
#include "Image.h"

void Image_ShowInformation( LPIMG_INFO_t pImageInstance )
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
