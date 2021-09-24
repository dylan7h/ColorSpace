#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "PNG.h"

using namespace std;

static char PNG_GetASCII2CHAR( uint8_t val );
static uint32_t PNG_ConvertNet2Host32(uint32_t val);

static void PNG_ShowSignature( LPPNG_t pInstance );
static void PNG_ShowImageHeader( LPPNG_t pInstance );

// static int32_t PNG_GetNextChunkOffset(int32_t CurrentChunkOffset);

int32_t PNG_Load(const char* lpszPath, LPPNG_t pInstance )
{
    int32_t ret = 0;
    FILE* fp;
    uint8_t* pPos;

    fp = fopen(lpszPath, "rb");
    if( fp != NULL )
    {
        size_t SizeOfFile;

        fseek( fp, 0, SEEK_END);
        SizeOfFile = ftell( fp );
        fseek( fp, 0, SEEK_SET);

        printf("[ PNG Info ]\n");
        printf("  File Size: %lu [ Byte ]\n\n", SizeOfFile);

        pInstance->pBuf = (uint8_t*)malloc( SizeOfFile );
        assert( pInstance->pBuf != NULL );

        /* 00. Load PNG Image From File */
        fread( pInstance->pBuf, 1, SizeOfFile, fp );

        /* 01. Show Signature */
        PNG_ShowSignature( pInstance );

        /* 02. Show Image Header Info */
        pPos = pInstance->pBuf + PNG_SIGNATURE_LEN;
        pInstance->pIHDR = (PNG_IHDR_t*)(pPos);
        PNG_ShowImageHeader( pInstance );

        fclose( fp );
    }
    else
    {
        ret = -1;
    }

    return ret;
}

static char PNG_GetASCII2CHAR( uint8_t val )
{
    char ret;

    if( ((val >= '0') && (val <= '9')) ||
        ((val >= 'a') && (val <= 'z')) ||
        ((val >= 'A') && (val <= 'Z')) )
    {
        ret = (char)val;
    }
    else
    {
        ret = '.';
    }

    return ret;
}

static uint32_t PNG_ConvertNet2Host32(uint32_t val)
{
    uint32_t ret;

    ret =   ((val & 0xFF000000) >> 24)  |
            ((val & 0x00FF0000) >> 8)   |
            ((val & 0x0000FF00) << 8)   |
            ((val & 0x000000FF) << 24);

    return ret;
}

static void PNG_ShowSignature( LPPNG_t pInstance )
{
    char buf[PNG_SIGNATURE_LEN];

    printf("  >> Signature: ");

    for(int i = 0; i < PNG_SIGNATURE_LEN; i++)
    {
        printf("%02X ", pInstance->pBuf[i]);
        buf[i] = PNG_GetASCII2CHAR( pInstance->pBuf[i] );
    }
    printf("| ");
    for(int i = 0; i < PNG_SIGNATURE_LEN; i++)
    {
        printf("%c", buf[i]);
    }

    printf("\n");
}

static void PNG_ShowImageHeader( LPPNG_t pInstance )
{
    printf("  >> Image Header\n");
    printf("    - Length                : %u\n", PNG_ConvertNet2Host32( pInstance->pIHDR->Length ) );
    printf("    - Type                  : ");

    for(int i = 0; i < PNG_CHUNK_TYPE_LEN; i++)
    {
        printf("%02X ", pInstance->pIHDR->ChunkType[i]);
    }
    printf("| ");
    for(int i = 0; i < PNG_CHUNK_TYPE_LEN; i++)
    {
        printf("%c", pInstance->pIHDR->ChunkType[i]);
    }
    printf("\n");

     printf("    - Width                 : %u\n", PNG_ConvertNet2Host32( pInstance->pIHDR->DAT.Width ) );
     printf("    - Height                : %u\n", PNG_ConvertNet2Host32( pInstance->pIHDR->DAT.Height ) );
     printf("    - Depth                 : %u\n", pInstance->pIHDR->DAT.Depth );
     printf("    - Color Type            : %u\n", pInstance->pIHDR->DAT.ColorType );
     printf("    - Compression Method    : %u\n", pInstance->pIHDR->DAT.CompressionMethod );
     printf("    - Filter Method         : %u\n", pInstance->pIHDR->DAT.FilterMethod );
     printf("    - Interlace Method      : %u\n", pInstance->pIHDR->DAT.InterlaceMethod );
}
