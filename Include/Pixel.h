#ifndef __PIXEL_H__
#define __PIXEL_H__

#include <stdint.h>
#include <stdbool.h>

#ifndef MAKEFOURCC
#   define MAKEFOURCC(ch0, ch1, ch2, ch3)                                               \
                ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) |           \
                ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */

#define SAFE_FREE(PTR)   do { free((PTR)); (PTR) = NULL; } while(0);


/* 
                            line stride
    ┼───────────────────────────────────────────────────────────────┼
                        width 
    ┼───────────────────────────────────────────────┼
    pixel stride       pixel depth                    line padding
    ┼───────────┼    ┼──────────────┼               ┼───────────────┼
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────────────┐ ┼
    │ R │ G │ B │ ■ │   │   │   │ ■ │   │   │   │ ■ │ ■■■■■■■■■■■■■ │ │
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───────────────┘ │
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────────────┐ │
    │   │   │   │   │   │   │   │   │   │   │   │   │ ■■■■■■■■■■■■■ │ │
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───────────────┘ │ height
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────────────┐ │
    │   │   │   │   │   │   │   │   │   │   │   │   │ ■■■■■■■■■■■■■ │ │
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───────────────┘ │
    ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────────────┐ │
    │   │   │   │ ■ │   │   │   │ ■ │   │   │   │ ■ │ ■■■■■■■■■■■■■ │ │
    └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───────────────┘ ┼
                                                ┼───┼
                                             pixel padding
 */

typedef struct tagPACKED_IMG
{
    uint32_t    nWidth;
    uint32_t    nHeight;

    uint32_t    nPixelDepth;    /* 8, 16, 24, 32 bit */
    uint32_t    nPixelPadding;
    uint32_t    nPixelStride;   

    uint32_t    nLinePadding;
    uint32_t    nLineStride;    /* x4 */

    uint32_t    nFourCC;

    void*       pBuffer;
} RGB_IMG_t, *LPRGB_IMG_t;


#endif // !__PIXEL_H__
