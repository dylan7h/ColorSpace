#ifndef __PNG__H__
#define __PNG__H__

#include <stdint.h>

/* 
    ┌─────────────────────────┬──────────────┬────────────┬───────────┐
    │       Signature         │     IHDR     │    IDAT    │    IEND   │
    │ 89 50 4E 47 0D 0A 1A 0A │ Image Header │ Image Data │ Image End │
    └─────────────────────────┴──────────────┴────────────┴───────────┘

    This table summarizes some properties of the standard chunk types.
    Critical chunks (must appear in this order, except PLTE
                    is optional):
   
           Name  Multiple  Ordering constraints
                   OK?
   
           IHDR    No      Must be first
           PLTE    No      Before IDAT
           IDAT    Yes     Multiple IDATs must be consecutive
           IEND    No      Must be last
   
    Ancillary chunks (need not appear in this order):
   
           Name  Multiple  Ordering constraints
                   OK?
   
           cHRM    No      Before PLTE and IDAT
           gAMA    No      Before PLTE and IDAT
           sBIT    No      Before PLTE and IDAT
           bKGD    No      After PLTE; before IDAT
           hIST    No      After PLTE; before IDAT
           tRNS    No      After PLTE; before IDAT
           pHYs    No      Before IDAT
           tIME    No      None
           tEXt    Yes     None
           zTXt    Yes     None

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

    Color   Allowed     Interpretation
    Type    Bit Depths
    0       1,2,4,8,16  Each pixel is a grayscale sample.
    
    2       8,16        Each pixel is an R,G,B triple.
    
    3       1,2,4,8     Each pixel is a palette index;
                        a PLTE chunk must appear.
    
    4       8,16        Each pixel is a grayscale sample,
                        followed by an alpha sample.
    
    6       8,16        Each pixel is an R,G,B triple,
                        followed by an alpha sample.

    Filter
    Type	Name	    Filter Function	                                                Reconstruction Function
    0	    None	    Filt(x) = Orig(x)	                                            Recon(x) = Filt(x)
    1	    Sub	        Filt(x) = Orig(x) - Orig(a)	                                    Recon(x) = Filt(x) + Recon(a)
    2	    Up	        Filt(x) = Orig(x) - Orig(b)	                                    Recon(x) = Filt(x) + Recon(b)
    3	    Average	    Filt(x) = Orig(x) - floor((Orig(a) + Orig(b)) / 2)	            Recon(x) = Filt(x) + floor((Recon(a) + Recon(b)) / 2)
    4	    Paeth	    Filt(x) = Orig(x) - PaethPredictor(Orig(a), Orig(b), Orig(c ))	Recon(x) = Filt(x) + PaethPredictor(Recon(a), Recon(b), Recon(c ))

    Compression method
        현재까지 PNG에서 표준으로 정의된 압축 방식은 0: DEFLATE 한 가지다.

    Interlace method
        인터레이스는 웹 페이지 등에 이미지를 표시할 때 이미지 로딩이 완료되기 전 먼저 해상도가 낮은 이미지를 보여주기 위하여 사용된다.
        현재까지 PNG에서 표준으로 정의된 인터레이스 방식은, 0 (No interlace), 1 (Adam7 interlace) 두 가지다.


    Image Data

    Encoding : Pixel Data(Original data) --> Filter --> Compression --> IDAT chunk data
    Decoding : IDAT chunk data --> Decompression --> Unfilter --> Pixel data


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
