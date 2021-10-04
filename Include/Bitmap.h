#ifndef __BITMAP__H__
#define __BITMAP__H__

#include <stdint.h>
#include <stdbool.h>

/*
 * ====================================================
    BITMAP INFO HEADER - Size
 * ====================================================
*/
#define BITMAP_INFO_HEADER_VERSION3_LENGTH  (40U)
#define BITMAP_INFO_HEADER_VERSION4_LENGTH  (108U)
#define BITMAP_INFO_HEADER_VERSION5_LENGTH  (124U)


/*
 * ====================================================
    BITMAP INFO HEADER - Compression
 * ====================================================
*/
#define BITMAP_INFO_HEADER_COMPRESSION_RGB      (0) /* Normal */
#define BITMAP_INFO_HEADER_COMPRESSION_RLE8     (1) /* Using 8 bpp */
#define BITMAP_INFO_HEADER_COMPRESSION_RLE4     (2) /* Using 4 bpp */
#define BITMAP_INFO_HEADER_COMPRESSION_BITFIELD (3) /* Using 16, 32 bpp */ 
#define BITMAP_INFO_HEADER_COMPRESSION_JPEG     (4) /* Included JPEG */
#define BITMAP_INFO_HEADER_COMPRESSION_PNG      (5) /* Included PNG */


/* 
    ┌────────────────────┐
    │   BITMAP HEADER    │
    ├────────────────────┤
    │ BITMAP INFO HEADER │
    ├────────────────────┤
    │   Color Palette    │
    ├────────────────────┤
    │    Pixel Data      │
    └────────────────────┘
 */
#pragma pack(push, 1)
typedef struct
{
    uint16_t Type;                  /* BMP Siganture            - Offset:   0 */
    uint32_t Size;                  /* Size Of This File        - Offset:   2 */
    uint16_t Reserved1;             /* Reserved                 - Offset:   6 */
    uint16_t Reserved2;             /* Reserved                 - Offset:   8 */
    uint32_t OffBits;               /* RAW Data Offset          - Offset:  10 */
} BMP_HEADER_t, *LPBMP_HEADER_t;    /* 14 Byte */
#pragma pack(pop)

typedef struct {
    uint32_t ciexyzX;
    uint32_t ciexyzY;
    uint32_t ciexyzZ;
} CIEXYZ;

typedef struct {
    CIEXYZ ciexyzRed;
    CIEXYZ ciexyzGreen;
    CIEXYZ ciexyzBlue;
} CIEXYZTRIPLE;

typedef union
{
    struct
    {
        uint32_t Size;              /* Size Of This Header      - Offset:  14 */
        uint32_t Width;             /* Width                    - Offset:  18 */
        uint32_t Height;            /* Heigth                   - Offset:  22 */
        uint16_t Planes;            /* Color Plane              - Offset:  26 */
        uint16_t BitCount;          /* Pixel Per Bits           - Offset:  28 */
        uint32_t Compression;       /* Type Of Compression      - Offset:  30 */
        uint32_t SizeImage;         /* Uncompressed Data Size   - Offset:  34 */
        uint32_t XPerlsPerMeter;    /* X( Pixel Per Meter )     - Offset:  38 */
        uint32_t YPerlsPerMeter;    /* Y( Pixel Per Meter )     - Offset:  42 */
        uint32_t ClrUsed;           /* Num Of Color Pallet      - Offset:  46 */
        uint32_t ClrImportant;      /* Num Of Important Color   - Offset:  50 */
    } V3;                           /* 40 Byte */

    struct
    {
        uint32_t        Size;
        uint32_t        Width;
        uint32_t        Height;
        uint16_t        Planes;
        uint16_t        BitCount;
        uint32_t        Compression;
        uint32_t        SizeImage;
        uint32_t        XPelsPerMeter;
        uint32_t        YPelsPerMeter;
        uint32_t        ClrUsed;
        uint32_t        ClrImportant;
        uint32_t        RedMask;
        uint32_t        GreenMask;
        uint32_t        BlueMask;
        uint32_t        AlphaMask;
        uint32_t        CSType;
        CIEXYZTRIPLE    Endpoints;
        uint32_t        GammaRed;
        uint32_t        GammaGreen;
        uint32_t        GammaBlue;
    } V4;   /* 108 Byte */

    struct
    {
        uint32_t        Size;
        uint32_t        Width;
        uint32_t        Height;
        uint16_t        Planes;
        uint16_t        BitCount;
        uint32_t        Compression;
        uint32_t        SizeImage;
        uint32_t        XPelsPerMeter;
        uint32_t        YPelsPerMeter;
        uint32_t        ClrUsed;
        uint32_t        ClrImportant;
        uint32_t        RedMask;
        uint32_t        GreenMask;
        uint32_t        BlueMask;
        uint32_t        AlphaMask;
        uint32_t        CSType;
        CIEXYZTRIPLE    Endpoints;
        uint32_t        GammaRed;
        uint32_t        GammaGreen;
        uint32_t        GammaBlue;
        uint32_t        Intent;
        uint32_t        ProfileData;
        uint32_t        ProfileSize;
        uint32_t        Reserved;
    } V5;   /* 124 Byte */
} BMP_INFO_HEADER_t, *LPBMP_INFO_HEADER_t;

typedef struct
{
    uint8_t*            pFileData;
    LPBMP_HEADER_t      pHeader;
    LPBMP_INFO_HEADER_t pInfoHeader;
    uint8_t*            pCollorPalette;
    uint8_t*            pPixelData;
} BITMAP_t, *LPBITMAP_t;

bool BMP_LoadFromFile(const char* lpszPath, LPBITMAP_t pBitmap);
void BMP_ShowInfo(LPBITMAP_t pBitmap);


#endif  //!__BITMAP__H__
