#include "Window.h"
#include "Texture.h"
#include "Image.h"

void Workspace( int argc, char* argv[] )
{
    SDL_Event e;
    bool bQuit = false;

    BITMAP_t BMP_RGB888;    IMG_t IMG_RGB888;
    BITMAP_t BMP_RGBA;      IMG_t IMG_RGBA;
    BITMAP_t BMP_RGBX;      IMG_t IMG_RGBX;
    BITMAP_t BMP_RGB565;    IMG_t IMG_RGB565;
    BITMAP_t BMP_ARGB1555;  IMG_t IMG_ARGB1555;
    BITMAP_t BMP_XRGB1555;  IMG_t IMG_XRGB1555;

    

    BMP_LoadFromFile("/home/dylan/workspace/00_github/ColorSpace/Images/Image_24bit.bmp", &BMP_RGB888);
    // BMP_LoadFromFile("/home/dylan/workspace/00_github/ColorSpace/Images/Image_RGB888.bmp", &BMP_RGB888);
    BMP_LoadFromFile("/home/dylan/workspace/00_github/ColorSpace/Images/Image_RGBA.bmp", &BMP_RGBA);
    BMP_LoadFromFile("/home/dylan/workspace/00_github/ColorSpace/Images/Image_RGBX.bmp", &BMP_RGBX);
    BMP_LoadFromFile("/home/dylan/workspace/00_github/ColorSpace/Images/RGB565.bmp", &BMP_RGB565);
    BMP_LoadFromFile("/home/dylan/workspace/00_github/ColorSpace/Images/ARGB1555.bmp", &BMP_ARGB1555);
    BMP_LoadFromFile("/home/dylan/workspace/00_github/ColorSpace/Images/XRGB1555.bmp", &BMP_XRGB1555);

    printf(">> BMP_RGB888\n");
    BMP_ShowInfo(&BMP_RGB888);

    printf(">> BMP_RGBA\n");
    BMP_ShowInfo(&BMP_RGBA);

    printf(">> BMP_RGBX\n");
    BMP_ShowInfo(&BMP_RGBX);

    printf(">> BMP_RGB565\n");
    BMP_ShowInfo(&BMP_RGB565);

    printf(">> BMP_ARGB1555\n");
    BMP_ShowInfo(&BMP_ARGB1555);

    printf(">> BMP_XRGB1555\n");
    BMP_ShowInfo(&BMP_XRGB1555);

    IMG_ConvertFromBitmap(&BMP_RGB888, &IMG_RGB888);

    CWindow MainWindow;
    MainWindow.Initialize( "Main Window", IMG_RGB888.nWidth, IMG_RGB888.nHeight );

    CTexture texture( &MainWindow );
    texture.CreateTexture( SDL_PIXELFORMAT_BGR24, IMG_RGB888.nWidth, IMG_RGB888.nHeight );


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
            texture.UpdateTextureData( IMG_RGB888.pPixelData, IMG_RGB888.nLineStride );
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
