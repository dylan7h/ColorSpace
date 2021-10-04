#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Bitmap.h"

bool BMP_LoadFromFile(const char* lpszPath, LPBITMAP_t pBitmap)
{
    bool bResult = false;

    if(lpszPath != NULL)
    {
        FILE* fp;
        size_t fileLen;

        fp = fopen(lpszPath, "rb");
        if(fp != NULL)
        {
            (void)fseek(fp, 0, SEEK_END);
            fileLen = ftell(fp);
            (void)fseek(fp, 0, SEEK_SET);

            pBitmap->pFileData = (uint8_t*)malloc( fileLen );
        }

        if(pBitmap->pFileData != NULL)
        {
            (void)fread((void*)pBitmap->pFileData, 1, fileLen, fp);

            pBitmap->pHeader        = (LPBMP_HEADER_t)pBitmap->pFileData;
            pBitmap->pInfoHeader    = (LPBMP_INFO_HEADER_t)(pBitmap->pFileData + sizeof(BMP_HEADER_t));
            pBitmap->pPixelData     = (uint8_t*)((uint64_t)pBitmap->pFileData + pBitmap->pHeader->OffBits);
            if(pBitmap->pInfoHeader->V3.ClrUsed != 0)
            {
                pBitmap->pCollorPalette = (uint8_t*)((uint64_t)pBitmap->pInfoHeader + pBitmap->pInfoHeader->V3.Size);
            }

            bResult = true;
        }
    }

    return bResult;
}

void BMP_ShowInfo(LPBITMAP_t pBitmap)
{
    assert(pBitmap != NULL);

    puts("[ BITMAP HEADER ]");
    printf("    - Type      : 0x%02X | %c%c\n", pBitmap->pHeader->Type, (char)(pBitmap->pHeader->Type), (char)(pBitmap->pHeader->Type >> 8));
    printf("    - Size      : %u\n", pBitmap->pHeader->Size);
    printf("    - OffBits   : %u\n\n", pBitmap->pHeader->OffBits);

    switch (pBitmap->pInfoHeader->V3.Size)
    {
        case BITMAP_INFO_HEADER_VERSION3_LENGTH:
        {
            puts("[ BITMAP INFO HEADER V3 ]");
            printf("    - Size              : %u\n",    pBitmap->pInfoHeader->V3.Size);
            printf("    - Width             : %u\n",    pBitmap->pInfoHeader->V3.Width);
            printf("    - Height            : %u\n",    pBitmap->pInfoHeader->V3.Height);
            printf("    - Planes            : %u\n",    pBitmap->pInfoHeader->V3.Planes);
            printf("    - BitCount          : %u\n",    pBitmap->pInfoHeader->V3.BitCount);
            printf("    - Compression       : %u\n",    pBitmap->pInfoHeader->V3.Compression);
            printf("    - SizeImage         : %u\n",    pBitmap->pInfoHeader->V3.SizeImage);
            printf("    - XPerlsPerMeter    : %u\n",    pBitmap->pInfoHeader->V3.XPerlsPerMeter);
            printf("    - YPerlsPerMeter    : %u\n",    pBitmap->pInfoHeader->V3.YPerlsPerMeter);
            printf("    - ClrUsed           : %u\n",    pBitmap->pInfoHeader->V3.ClrUsed);
            printf("    - ClrImportant      : %u\n\n",  pBitmap->pInfoHeader->V3.ClrImportant);
        }
        break;

        case BITMAP_INFO_HEADER_VERSION4_LENGTH:
        {
            puts("[ BITMAP INFO HEADER V4 ]");
            printf("    - Size              : %u\n",        pBitmap->pInfoHeader->V4.Size);
            printf("    - Width             : %u\n",        pBitmap->pInfoHeader->V4.Width);
            printf("    - Height            : %u\n",        pBitmap->pInfoHeader->V4.Height);
            printf("    - Planes            : %u\n",        pBitmap->pInfoHeader->V4.Planes);
            printf("    - BitCount          : %u\n",        pBitmap->pInfoHeader->V4.BitCount);
            printf("    - Compression       : %u\n",        pBitmap->pInfoHeader->V4.Compression);
            printf("    - SizeImage         : %u\n",        pBitmap->pInfoHeader->V4.SizeImage);
            printf("    - XPerlsPerMeter    : %u\n",        pBitmap->pInfoHeader->V4.XPelsPerMeter);
            printf("    - YPerlsPerMeter    : %u\n",        pBitmap->pInfoHeader->V4.YPelsPerMeter);
            printf("    - ClrUsed           : %u\n",        pBitmap->pInfoHeader->V4.ClrUsed);
            printf("    - ClrImportant      : %u\n",        pBitmap->pInfoHeader->V4.ClrImportant);
            printf("    - RedMask           : 0x%08X\n",    pBitmap->pInfoHeader->V4.RedMask);
            printf("    - GreenMask         : 0x%08X\n",    pBitmap->pInfoHeader->V4.GreenMask);
            printf("    - BlueMask          : 0x%08X\n",    pBitmap->pInfoHeader->V4.BlueMask);
            printf("    - AlphaMask         : 0x%08X\n",    pBitmap->pInfoHeader->V4.AlphaMask);
            printf("    - CSType            : %u\n",        pBitmap->pInfoHeader->V4.CSType);
            printf("    - Endpoints.Red     : [ x - %10u, y - %10u, z - %10u ]\n",
                                                            pBitmap->pInfoHeader->V4.Endpoints.ciexyzRed.ciexyzX,
                                                            pBitmap->pInfoHeader->V4.Endpoints.ciexyzRed.ciexyzY,
                                                            pBitmap->pInfoHeader->V4.Endpoints.ciexyzRed.ciexyzZ);
            printf("    - Endpoints.Green   : [ x - %10u, y - %10u, z - %10u ]\n",
                                                            pBitmap->pInfoHeader->V4.Endpoints.ciexyzGreen.ciexyzX,
                                                            pBitmap->pInfoHeader->V4.Endpoints.ciexyzGreen.ciexyzY,
                                                            pBitmap->pInfoHeader->V4.Endpoints.ciexyzGreen.ciexyzZ);
            printf("    - Endpoints.Blue    : [ x - %10u, y - %10u, z - %10u ]\n",
                                                            pBitmap->pInfoHeader->V4.Endpoints.ciexyzBlue.ciexyzX,
                                                            pBitmap->pInfoHeader->V4.Endpoints.ciexyzBlue.ciexyzY,
                                                            pBitmap->pInfoHeader->V4.Endpoints.ciexyzBlue.ciexyzZ);
            printf("    - GammaRed          : %u\n",        pBitmap->pInfoHeader->V4.GammaRed);
            printf("    - GammaGreen        : %u\n",        pBitmap->pInfoHeader->V4.GammaGreen);
            printf("    - GammaBlue         : %u\n\n",      pBitmap->pInfoHeader->V4.GammaBlue);
        }
        break;

        case BITMAP_INFO_HEADER_VERSION5_LENGTH:
        {
            puts("[ BITMAP INFO HEADER V5 ]");
            printf("    - Size              : %u\n",        pBitmap->pInfoHeader->V5.Size);
            printf("    - Width             : %u\n",        pBitmap->pInfoHeader->V5.Width);
            printf("    - Height            : %u\n",        pBitmap->pInfoHeader->V5.Height);
            printf("    - Planes            : %u\n",        pBitmap->pInfoHeader->V5.Planes);
            printf("    - BitCount          : %u\n",        pBitmap->pInfoHeader->V5.BitCount);
            printf("    - Compression       : %u\n",        pBitmap->pInfoHeader->V5.Compression);
            printf("    - SizeImage         : %u\n",        pBitmap->pInfoHeader->V5.SizeImage);
            printf("    - XPerlsPerMeter    : %u\n",        pBitmap->pInfoHeader->V5.XPelsPerMeter);
            printf("    - YPerlsPerMeter    : %u\n",        pBitmap->pInfoHeader->V5.YPelsPerMeter);
            printf("    - ClrUsed           : %u\n",        pBitmap->pInfoHeader->V5.ClrUsed);
            printf("    - ClrImportant      : %u\n",        pBitmap->pInfoHeader->V5.ClrImportant);
            printf("    - RedMask           : 0x%08X\n",    pBitmap->pInfoHeader->V5.RedMask);
            printf("    - GreenMask         : 0x%08X\n",    pBitmap->pInfoHeader->V5.GreenMask);
            printf("    - BlueMask          : 0x%08X\n",    pBitmap->pInfoHeader->V5.BlueMask);
            printf("    - AlphaMask         : 0x%08X\n",    pBitmap->pInfoHeader->V5.AlphaMask);
            printf("    - CSType            : %u\n",        pBitmap->pInfoHeader->V5.CSType);
            printf("    - Endpoints.Red     : [ x - %10u, y - %10u, z - %10u ]\n",
                                                            pBitmap->pInfoHeader->V5.Endpoints.ciexyzRed.ciexyzX,
                                                            pBitmap->pInfoHeader->V5.Endpoints.ciexyzRed.ciexyzY,
                                                            pBitmap->pInfoHeader->V5.Endpoints.ciexyzRed.ciexyzZ);
            printf("    - Endpoints.Green   : [ x - %10u, y - %10u, z - %10u ]\n",
                                                            pBitmap->pInfoHeader->V5.Endpoints.ciexyzGreen.ciexyzX,
                                                            pBitmap->pInfoHeader->V5.Endpoints.ciexyzGreen.ciexyzY,
                                                            pBitmap->pInfoHeader->V5.Endpoints.ciexyzGreen.ciexyzZ);
            printf("    - Endpoints.Blue    : [ x - %10u, y - %10u, z - %10u ]\n",
                                                            pBitmap->pInfoHeader->V5.Endpoints.ciexyzBlue.ciexyzX,
                                                            pBitmap->pInfoHeader->V5.Endpoints.ciexyzBlue.ciexyzY,
                                                            pBitmap->pInfoHeader->V5.Endpoints.ciexyzBlue.ciexyzZ);
            printf("    - GammaRed          : %u\n",        pBitmap->pInfoHeader->V5.GammaRed);
            printf("    - GammaGreen        : %u\n",        pBitmap->pInfoHeader->V5.GammaGreen);
            printf("    - Intent            : %u\n",        pBitmap->pInfoHeader->V5.Intent);
            printf("    - ProfileData       : %u\n",        pBitmap->pInfoHeader->V5.ProfileData);
            printf("    - ProfileSize       : %u\n\n",      pBitmap->pInfoHeader->V5.ProfileSize);
        }
        break;
    
        default:
        break;
    }
}
