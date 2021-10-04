#ifndef __IMAGE__H__
#define __IMAGE__H__

#include <stdint.h>
#include "Bitmap.h"

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

typedef struct tagIMG
{
    uint32_t                nWidth;
    uint32_t                nHeight;

    uint32_t                nPixelDepth;    /* 8, 16, 24, 32 bit */
    uint32_t                nPixelPadding;
    uint32_t                nPixelStride;   

    uint32_t                nLinePadding;
    uint32_t                nLineStride;    /* x4 */

    char                    lpszFormat[10];

    void*                   pBuffer;
} IMG_t, *LPIMG_t;

void IMG_SetFormatOfBitmap(LPBITMAP_t lpBMP, LPIMG_t lpIMG);
bool IMG_ConvertBMP2IMG(LPBITMAP_t lpBMP, LPIMG_t lpIMG);

#endif  //!__IMAGE__H__
