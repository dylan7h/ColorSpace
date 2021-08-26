#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include "Window.h"

extern void Workspace( int argc, char* argv[] );

int main( int argc, char* argv[] )
{
    AllocConsole();

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((intptr_t) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((intptr_t) handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, NULL, _IONBF, 128);
    *stdin = *hf_in;

    CWindow::SDL_StartUp();

    Workspace( argc, argv );

    CWindow::SDL_Release();

    return 0;
}
