#include <Windows.h>
#include "Window.h"

extern void Workspace( int argc, char* argv[] );

int main( int argc, char* argv[] )
{
    AllocConsole();

    freopen("CONIN$",   "rb", stdin);
    freopen("CONOUT$",  "wb", stdout);
    freopen("CONOUT$",  "wb", stderr);

    CWindow::SDL_StartUp();

    Workspace( argc, argv );

    CWindow::SDL_Release();

    FreeConsole();

    return 0;
}
