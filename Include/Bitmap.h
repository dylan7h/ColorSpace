#ifndef __BITMAP__H__
#define __BITMAP__H__

#include <stdint.h>
#include "Image.h"

/* BMP Signature : BM */
#define BMP_SIGNATURE           ( 0x4D42U ) 

/* Important Header Size Info */
#define SIZE_OF_BMP_FILE_HEADER ( 14U )
#define SIZE_OF_BMP_INFO_HEADER ( 40U )
#define SIZE_OF_BMP_HEADER      ( 54U )

/* Flags of biCompression of Bitmap Info */
#define BI_COMP_RGB              ( 0U )
#define BI_COMP_RLE8             ( 1U )
#define BI_COMP_RLE4             ( 2U )
#define BI_COMP_BITFIELDS        ( 3U )
#define BI_COMP_JPEG             ( 4U )
#define BI_COMP_PNG              ( 5U )

#pragma pack(push, 2)
typedef struct
{
    uint16_t bfType;            /* BMP Siganture            - Offset:   0 */
    uint32_t bfSize;            /* Size Of This File        - Offset:   2 */
    uint16_t bfReserved1;       /* Reserved                 - Offset:   6 */
    uint16_t bfReserved2;       /* Reserved                 - Offset:   8 */
    uint32_t bfOffBits;         /* RAW Data Offset          - Offset:  10 */
} BMP_FILE_t;                   /* 14 Byte */
#pragma pack(pop)

typedef struct
{
    uint32_t biSize;            /* Size Of This Header      - Offset:  14 */
    uint32_t biWidth;           /* Width                    - Offset:  18 */
    uint32_t biHeight;          /* Heigth                   - Offset:  22 */
    uint16_t biPlanes;          /* Color Plane              - Offset:  26 */
    uint16_t biBitCount;        /* Pixel Per Bits           - Offset:  28 */
    uint32_t biCompression;     /* Type Of Compression      - Offset:  30 */
    uint32_t biSizeImage;       /* Uncompressed Data Size   - Offset:  34 */
    uint32_t biXPerlsPerMeter;  /* X( Pixel Per Meter )     - Offset:  38 */
    uint32_t biYPerlsPerMeter;  /* Y( Pixel Per Meter )     - Offset:  42 */
    uint32_t biClrUsed;         /* Num Of Color Pallet      - Offset:  46 */
    uint32_t biClrImportant;    /* Num Of Important Color   - Offset:  50 */
} BMP_INFO_t;                   /* 40 Byte */

typedef struct
{
    BMP_FILE_t File;
    BMP_INFO_t Info;
    IMG_INFO_t Image;
} BMP_t, *LPBMP_t;

void BMP_LoadFromFile( const char lpszPath[], LPBMP_t pBMPInstance );
void BMP_ApplyPalette( LPBMP_t pDstBMPInstance, LPBMP_t pSrcBMPInstance );
void BMP_ShowInformation( LPBMP_t pBMPInstance );

#endif  //!__BITMAP__H__
