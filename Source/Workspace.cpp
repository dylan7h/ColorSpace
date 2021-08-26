#include "Window.h"
#include "Texture.h"
#include "Bitmap.h"

void Workspace( int argc, char* argv[] )
{
    CWindow MainWindow, SubWindow;
    SDL_Event e;
    bool bQuit = false;


    MainWindow.Initialize( "Main Window", 640, 480 );
    SubWindow.Initialize( "Sub Window", 640, 480 );

    BMP_t bmp;
    BMP_LoadFromFile( "F:/00_workspace/ColorSpace/Images/Image_24bit.bmp", &bmp );

    CTexture texture( &MainWindow );
    texture.CreateTexture( SDL_PIXELFORMAT_BGR24, bmp.Image.nWidth, bmp.Image.nHeight );

    BMP_ShowInformation(&bmp);
    Image_ShowInformation(&bmp.Image);

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
            texture.UpdateTextureData( bmp.Image.pPixels, bmp.Image.nStride );
        }

        CWindow::ClearScreen();

        /* Render */
        {
            SDL_Rect rtScreen;

            texture.GetScreenRect(&rtScreen);
            texture.Render(0, 0, &rtScreen, 0.0, nullptr, SDL_FLIP_VERTICAL);
        }

        CWindow::RenderPresent();
        bQuit = CWindow::IsAllWindowClosed();
    }
}
