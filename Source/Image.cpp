#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Image.h"

static uint8_t  IMG_GetCountOfMaskBits(uint32_t val);
static uint8_t  IMG_GetPosOfMaskBits(uint32_t val);
static void     IMG_SetFormatOfBitmap(LPBITMAP_t lpBMP, LPIMG_t lpIMG);

bool IMG_ConvertFromBitmap(LPBITMAP_t lpBMP, LPIMG_t lpIMG)
{
    uint32_t i, j;

    lpIMG->nWidth       = lpBMP->pInfoHeader->V3.Width;
    lpIMG->nHeight      = lpBMP->pInfoHeader->V3.Height;
    lpIMG->nPixelDepth  = lpBMP->pInfoHeader->V3.BitCount;
    switch (lpBMP->pInfoHeader->V3.Size)
    {
        case BITMAP_INFO_HEADER_VERSION3_LENGTH:
        {
            lpIMG->nPixelStride  = lpBMP->pInfoHeader->V3.BitCount;

        }
        break;

        case BITMAP_INFO_HEADER_VERSION4_LENGTH:
        case BITMAP_INFO_HEADER_VERSION5_LENGTH:
        {
            lpIMG->nPixelStride  =  IMG_GetCountOfMaskBits( lpBMP->pInfoHeader->V4.AlphaMask )  +
                                    IMG_GetCountOfMaskBits( lpBMP->pInfoHeader->V4.RedMask )    +
                                    IMG_GetCountOfMaskBits( lpBMP->pInfoHeader->V4.GreenMask )  +
                                    IMG_GetCountOfMaskBits( lpBMP->pInfoHeader->V4.BlueMask );
        }
        break;
    
        default:
        break;
    }
    lpIMG->nPixelPadding = lpIMG->nPixelDepth - lpIMG->nPixelStride;

    lpIMG->nLinePadding = ( ( lpIMG->nWidth * lpIMG->nPixelDepth ) / 8 ) % 4;
    lpIMG->nLineStride = ((lpIMG->nWidth * lpIMG->nPixelDepth) / 8) + lpIMG->nLinePadding;

    IMG_SetFormatOfBitmap(lpBMP, lpIMG);

    lpIMG->nImageSize = lpBMP->pInfoHeader->V3.SizeImage;
    lpIMG->pPixelData = (uint8_t*)malloc(lpIMG->nImageSize);
    assert(lpIMG->pPixelData != NULL);

    for(j = 0; j < lpIMG->nHeight; j++)
    {
        for(i = 0; i < lpIMG->nLineStride; i++)
        {
            lpIMG->pPixelData[(j * lpIMG->nLineStride) + i] = lpBMP->pPixelData[((lpIMG->nHeight - j - 1 ) * lpIMG->nLineStride) + i];
        }
    }

    return false;
}

SDL_PixelFormatEnum IMG_GetPixelFormatOfSDL(LPIMG_t lpIMG)
{
    SDL_PixelFormatEnum ePixelFormat = SDL_PIXELFORMAT_UNKNOWN;

    if(strcmp(lpIMG->lpszFormat, "RGB332") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_RGB332;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "RGB444") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_RGB444;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "RGB555") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_RGB555;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "BGR555") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_BGR555;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "ARGB4444") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_ARGB4444;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "RGBA4444") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_RGBA4444;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "ABGR4444") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_ABGR4444;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "BGRA4444") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_BGRA4444;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "ARGB1555") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_ARGB1555;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "RGBA5551") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_RGBA5551;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "ABGR1555") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_ABGR1555;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "BGRA5551") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_BGRA5551;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "RGB565") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_RGB565;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "BGR565") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_BGR565;
        goto EXIT_FUNCTION;
    }

    if((strcmp(lpIMG->lpszFormat, "RGB888") == 0) || (strcmp(lpIMG->lpszFormat, "RGB24") == 0))
    {
        ePixelFormat = SDL_PIXELFORMAT_RGB24;
        goto EXIT_FUNCTION;
    }

    if((strcmp(lpIMG->lpszFormat, "BGR888") == 0) || (strcmp(lpIMG->lpszFormat, "BGR24") == 0))
    {
        ePixelFormat = SDL_PIXELFORMAT_BGR24;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "XRGB8888") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_RGB888;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "RGBX8888") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_RGBX8888;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "XBGR8888") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_BGR888;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "BGRX8888") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_BGRX8888;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "ARGB8888") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_ARGB8888;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "RGBA8888") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_RGBA8888;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "ABGR8888") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_ABGR8888;
        goto EXIT_FUNCTION;
    }

    if(strcmp(lpIMG->lpszFormat, "BGRA8888") == 0)
    {
        ePixelFormat = SDL_PIXELFORMAT_BGRA8888;
        goto EXIT_FUNCTION;
    }

EXIT_FUNCTION:
    return ePixelFormat;
}

static uint8_t IMG_GetCountOfMaskBits(uint32_t val)
{
    uint32_t ret = 0;

    for(uint32_t i = 0; i < 32; i++)
    {
        ret += (val >> i) & 1;
    }

    return ret; 
}

static uint8_t IMG_GetPosOfMaskBits(uint32_t val)
{
    uint32_t i = 0;

    for(i = 0; i < 32; i++)
    {
        if(((val >> i) & 1) != 0)
        {
            break;
        }
    }

    return (i == 32) ? 0 : i; 
}

static void IMG_SetFormatOfBitmap(LPBITMAP_t lpBMP, LPIMG_t lpIMG)
{
    switch (lpBMP->pInfoHeader->V3.Size)
    {
        case BITMAP_INFO_HEADER_VERSION3_LENGTH:
        {
            sprintf(lpIMG->lpszFormat, "BGR24");
        }
        break;

        case BITMAP_INFO_HEADER_VERSION4_LENGTH:
        case BITMAP_INFO_HEADER_VERSION5_LENGTH:
        {
            uint32_t SumOfBits;

            SumOfBits = IMG_GetCountOfMaskBits( lpBMP->pInfoHeader->V4.RedMask )    + 
                        IMG_GetCountOfMaskBits( lpBMP->pInfoHeader->V4.GreenMask )  + 
                        IMG_GetCountOfMaskBits( lpBMP->pInfoHeader->V4.BlueMask )   + 
                        IMG_GetCountOfMaskBits( lpBMP->pInfoHeader->V4.AlphaMask );

            if(lpBMP->pInfoHeader->V4.BitCount == SumOfBits)
            {
                if(lpBMP->pInfoHeader->V4.AlphaMask != 0)
                {
                    /* ARGB or RGBA */
                    if(lpBMP->pInfoHeader->V4.RedMask > lpBMP->pInfoHeader->V4.BlueMask)
                    {
                        if(lpBMP->pInfoHeader->V4.AlphaMask > lpBMP->pInfoHeader->V4.RedMask)
                        {
                            sprintf(lpIMG->lpszFormat, "BGRA%u%u%u%u", 
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.BlueMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.GreenMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.RedMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.AlphaMask));
                        }
                        else
                        {
                            sprintf(lpIMG->lpszFormat, "ABGR%u%u%u%u", 
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.AlphaMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.BlueMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.GreenMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.RedMask));
                        }
                    }
                    else
                    {
                        if(lpBMP->pInfoHeader->V4.AlphaMask > lpBMP->pInfoHeader->V4.RedMask)
                        {
                            sprintf(lpIMG->lpszFormat, "RGBA%u%u%u%u", 
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.RedMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.GreenMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.BlueMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.AlphaMask));
                        }
                        else
                        {
                            sprintf(lpIMG->lpszFormat, "ARGB%u%u%u%u", 
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.AlphaMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.RedMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.GreenMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.BlueMask));
                        }
                    }
                }
                else
                {
                    /* RGB */
                    if(lpBMP->pInfoHeader->V4.RedMask > lpBMP->pInfoHeader->V4.BlueMask)
                    {
                        sprintf(lpIMG->lpszFormat, "BGR%u%u%u", 
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.BlueMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.GreenMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.RedMask));
                    }
                    else
                    {
                        sprintf(lpIMG->lpszFormat, "RGB%u%u%u", 
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.RedMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.GreenMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.BlueMask));
                    }
                }
            }
            else
            {
                /* XRGB or RGBX */
                if(lpBMP->pInfoHeader->V4.RedMask > lpBMP->pInfoHeader->V4.BlueMask)
                {
                    if((lpBMP->pInfoHeader->V4.BlueMask & 1) != 0 )
                    {
                        sprintf(lpIMG->lpszFormat, "BGRX%u%u%u%u", 
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.BlueMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.GreenMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.RedMask),
                                lpBMP->pInfoHeader->V4.BitCount - SumOfBits);
                    }
                    else
                    {
                        sprintf(lpIMG->lpszFormat, "XBGR%u%u%u%u", 
                                lpBMP->pInfoHeader->V4.BitCount - SumOfBits,
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.BlueMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.GreenMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.RedMask));
                    }
                }
                else
                {
                    if((lpBMP->pInfoHeader->V4.RedMask & 1) != 0 )
                    {
                        sprintf(lpIMG->lpszFormat, "RGBX%u%u%u%u", 
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.RedMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.GreenMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.BlueMask),
                                lpBMP->pInfoHeader->V4.BitCount - SumOfBits);
                    }
                    else
                    {
                        sprintf(lpIMG->lpszFormat, "XRGB%u%u%u%u", 
                                lpBMP->pInfoHeader->V4.BitCount - SumOfBits,
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.RedMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.GreenMask),
                                IMG_GetCountOfMaskBits(lpBMP->pInfoHeader->V4.BlueMask));
                    }
                }
            }
        }
        break;
    
        default:
            fputs ("Invalid Bitmap Info Version...\n",stderr);
            abort();
        break;
    }

    printf(">> %s\n", lpIMG->lpszFormat);
}


