#ifndef __PNG__H__
#define __PNG__H__

#include <stdint.h>

/* 
    ┌─────────────────────────┬──────────────┬────────────┬───────────┐
    │       Signature         │     IHDR     │    IDAT    │    IEND   │
    │ 89 50 4E 47 0D 0A 1A 0A │ Image Header │ Image Data │ Image End │
    └─────────────────────────┴──────────────┴────────────┴───────────┘
    Chunk
    {
        Length (4 byte),
        Chunk Type (4 byte),
        Chunk Data (length byte),
        CRC (4byte)
    }

    Image Header
    {
        Length : 00 00 00 0D (13 byte),
        Chunk Type : IHDR,
        Chunk Data ( 13 byte ),
        {
            Width (4 byte),
            Height (4 byte),
            Bit depth (1 byte),
            Color Type (1 byte),
            Compression method (1 byte),
            Filter method (1 byte),
            Interlace method (1 byte),
        }
        CRC
    }

    Image Data

    Encoding : Pixel Data(Original data) --> Filter --> Compression --> IDAT chunk data
    Decoding : IDAT chunk data --> Decompression --> Unfilter --> Pixel data

    Type	Name	    Filter Function	                                                Reconstruction Function
    0	    None	    Filt(x) = Orig(x)	                                            Recon(x) = Filt(x)
    1	    Sub	        Filt(x) = Orig(x) - Orig(a)	                                    Recon(x) = Filt(x) + Recon(a)
    2	    Up	        Filt(x) = Orig(x) - Orig(b)	                                    Recon(x) = Filt(x) + Recon(b)
    3	    Average	    Filt(x) = Orig(x) - floor((Orig(a) + Orig(b)) / 2)	            Recon(x) = Filt(x) + floor((Recon(a) + Recon(b)) / 2)
    4	    Paeth	    Filt(x) = Orig(x) - PaethPredictor(Orig(a), Orig(b), Orig(c ))	Recon(x) = Filt(x) + PaethPredictor(Recon(a), Recon(b), Recon(c ))

    Image End
    {
        Length : 00 00 00 00 (0 byte),
        Chunk Type : IEND,
        Chunk Data (0 byte),
        CRC
    }
 */

#define PNG_SIGNATURE_LEN       (8)
#define PNG_CHUNK_TYPE_LEN      (4)

#pragma pack(push, 1)
typedef struct 
{
    uint32_t    Length;
    char        ChunkType[PNG_CHUNK_TYPE_LEN];
    struct {
        uint32_t Width;
        uint32_t Height;
        uint8_t  Depth;
        uint8_t  ColorType;
        uint8_t  CompressionMethod;
        uint8_t  FilterMethod;
        uint8_t  InterlaceMethod;
    } DAT;
    uint32_t CRC;
} PNG_IHDR_t;
#pragma pack(pop)

typedef struct
{
    PNG_IHDR_t* pIHDR;
    uint32_t    OffsetIDAT;
    uint32_t    OffsetIEND;
    uint32_t    NumOfIDAT;
    uint8_t*    pBuf;
} PNG_t, *LPPNG_t;


int32_t PNG_Load(const char* lpszPath, LPPNG_t pInstance );

#endif  //!__PNG__H__
