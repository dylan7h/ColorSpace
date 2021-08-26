#include "Window.h"
#include "Texture.h"

void Workspace( int argc, char* argv[] )
{
    CWindow MainWindow, SubWindow;
    SDL_Event e;
    bool bQuit = false;


    MainWindow.Initialize( "Main Window", 640, 480 );
    SubWindow.Initialize( "Sub Window", 640, 480 );

    CTexture png( &MainWindow );
    png.LoadFromFile( "F:/00_workspace/ColorSpace/Images/Image_png.png" );

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

        CWindow::ClearScreen();

        png.Render(0, 0);

        CWindow::RenderPresent();
        
        bQuit = CWindow::IsAllWindowClosed();
    }
}
