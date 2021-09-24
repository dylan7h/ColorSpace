#include "Window.h"
#include "Texture.h"
#include "Bitmap.h"
#include "PNG.h"

void Workspace( int argc, char* argv[] )
{
    SDL_Event e;
    bool bQuit = false;

    BMP_t SrcBMP, DstBMP;
    LPBMP_t lpDispBMP = &SrcBMP;
    
    BMP_LoadFromFile( argv[1], &SrcBMP );
    BMP_ShowInformation(&SrcBMP);
    IMG_RGB_ShowInformation(&SrcBMP.Image);
    
    if( SrcBMP.Image.nPixelPerBit < 24 )
    {
        BMP_ApplyPalette( &DstBMP, &SrcBMP );
        lpDispBMP = &DstBMP;

        BMP_ShowInformation(&DstBMP);
        IMG_RGB_ShowInformation(&DstBMP.Image);
    }

    CWindow MainWindow;
    MainWindow.Initialize( "Main Window", lpDispBMP->Image.nWidth, lpDispBMP->Image.nHeight );

    CTexture texture( &MainWindow );
    texture.CreateTexture( SDL_PIXELFORMAT_BGR24, lpDispBMP->Image.nWidth, lpDispBMP->Image.nHeight );

    YUV_t YUVInstance;
    IMG_ConvertDomainRGBtoYUV( &lpDispBMP->Image, &YUVInstance );

    IMG_ConvertYUVtoYUV444( &YUVInstance );
    SAFE_FREE(YUVInstance.pYUV);

    IMG_ConvertYUVtoYUYV( &YUVInstance );
    SAFE_FREE(YUVInstance.pYUV);

    IMG_ConvertYUVtoUYVY( &YUVInstance );
    SAFE_FREE(YUVInstance.pYUV);

    IMG_ConvertYUVtoNV12( &YUVInstance );
    SAFE_FREE(YUVInstance.pYUV);

    IMG_ConvertYUVtoNV21( &YUVInstance );
    SAFE_FREE(YUVInstance.pYUV);

    PNG_t pngInstance;
    PNG_Load( argv[2], &pngInstance );

    while( bQuit == false )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT )
            {
                bQuit = true;
            }

            CWindow::UpdateWindowState( e );
        }

        /* Update Data */
        {
            texture.UpdateTextureData( lpDispBMP->Image.pPixels, lpDispBMP->Image.nStride );
        }

        CWindow::ClearScreen();

        /* Render */
        {
            SDL_Rect rtScreen;

            texture.GetScreenRect(&rtScreen);
            texture.Render(0, 0, &rtScreen, 0.0, nullptr, SDL_FLIP_NONE);
        }

        CWindow::RenderPresent();
        bQuit = CWindow::IsAllWindowClosed();
    }
}
