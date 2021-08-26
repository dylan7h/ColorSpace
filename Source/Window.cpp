#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <SDL_image.h>
#include "Window.h"
using namespace std;

vector<CWindow*> CWindow::vecInstance;

CWindow::CWindow()
{
    this->pRenderer         = nullptr;
    this->pWindow           = nullptr;

    this->nWidth            = 0;
    this->nHeight           = 0;

    this->bMouseFocus       = false;
	this->bKeyboardFocus    = false;
	this->bFullScreen       = false;
    this->bMinimized        = false;
	this->bShown            = false;
	this->nWindowID         = -1;
}

CWindow::~CWindow()
{
    this->DeInitialize();
}

void CWindow::SDL_StartUp(void)
{
    assert( SDL_Init( SDL_INIT_VIDEO ) >= 0 );

    if( SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) == SDL_FALSE )
    {
        printf( "Warning: Linear texture filtering not enabled!" );
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    assert( ( IMG_Init( imgFlags ) & imgFlags ) != 0 );
}

void CWindow::SDL_Release(void)
{
    IMG_Quit();
    SDL_Quit();
}


void CWindow::Initialize(const char lpszTitle[], int nWidth, int nHeight)
{
    assert( lpszTitle != nullptr );
    assert( nWidth > 0 );
    assert( nHeight > 0 );

    if( (this->pWindow == nullptr) && (this->pRenderer == nullptr) )
    {
        this->pWindow = SDL_CreateWindow( lpszTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, nWidth, nHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
        assert( this->pWindow != nullptr );

        this->pRenderer = SDL_CreateRenderer( this->pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
        assert( this->pRenderer != nullptr );

        SDL_SetRenderDrawColor( this->pRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

        CWindow::vecInstance.push_back( this );

        this->nWindowID = SDL_GetWindowID( this->pWindow );
        this->nWidth    = nWidth;
        this->nHeight   = nHeight;
        this->bShown    = true;
    }
}

void CWindow::DeInitialize(void)
{
    if( (this->pWindow != nullptr) || (this->pRenderer != nullptr) )
    {
        SDL_DestroyRenderer( this->pRenderer );
        SDL_DestroyWindow( this->pWindow );
        this->pRenderer = nullptr;
        this->pWindow = nullptr;

        this->nWidth            = 0;
        this->nHeight           = 0;

        this->bMouseFocus       = false;
        this->bKeyboardFocus    = false;
        this->bFullScreen       = false;
        this->bShown            = false;
        this->nWindowID         = -1;
    }
}

void CWindow::UpdateWindowState( SDL_Event& e )
{
    vector<CWindow*>::iterator it;

    for( it = CWindow::vecInstance.begin(); it != CWindow::vecInstance.end(); ++it )
    {
        CWindow* pInstance = *it;

        if( ( e.type == SDL_WINDOWEVENT ) && ( e.window.windowID == pInstance->nWindowID ) )
        {
            switch ( e.window.event )
            {
                case SDL_WINDOWEVENT_SHOWN:
                    pInstance->bShown = true;
                break;

                case SDL_WINDOWEVENT_HIDDEN:
                    pInstance->bShown = false;
                break;

                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    pInstance->nWidth   = e.window.data1;
                    pInstance->nHeight  = e.window.data2;
                    // render
                break;

                case SDL_WINDOWEVENT_EXPOSED:
                    // render
                break;

                case SDL_WINDOWEVENT_ENTER:
                    pInstance->bMouseFocus = true;
                break;

                case SDL_WINDOWEVENT_LEAVE:
                    pInstance->bMouseFocus = false;
                break;

                case SDL_WINDOWEVENT_FOCUS_GAINED:
                    pInstance->bKeyboardFocus = true;
                break;

                case SDL_WINDOWEVENT_FOCUS_LOST:
                    pInstance->bKeyboardFocus = false;
                break;

                case SDL_WINDOWEVENT_MINIMIZED:
                    pInstance->bMinimized = true;
                break;

                case SDL_WINDOWEVENT_MAXIMIZED:
                    pInstance->bMinimized = false;
                break;

                case SDL_WINDOWEVENT_RESTORED:
                    pInstance->bMinimized = false;
                break;

                case SDL_WINDOWEVENT_CLOSE:
                    SDL_HideWindow( pInstance->pWindow );
                    pInstance->DeInitialize();
                break;
            
                default:
                    /* Empty */
                break;
            }
        }
    }

    it = CWindow::vecInstance.begin();
    while( it != CWindow::vecInstance.end() )
    {
        CWindow* pInstance = *it;

        if( pInstance->pWindow == nullptr )
        {
            it = CWindow::vecInstance.erase( it );
        }
        else
        {
            ++it;
        }
    }
}

void CWindow::ClearScreen(void)
{
    vector<CWindow*>::iterator it;

    for( it = CWindow::vecInstance.begin(); it != CWindow::vecInstance.end(); ++it )
    {
        CWindow* pInstance = *it;

        if( pInstance->bMinimized == false )
        {
            SDL_SetRenderDrawColor( pInstance->pRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( pInstance->pRenderer );
        }
    }

}

void CWindow::RenderPresent(void)
{
    vector<CWindow*>::iterator it;

    for( it = CWindow::vecInstance.begin(); it != CWindow::vecInstance.end(); ++it )
    {
        CWindow* pInstance = *it;

        if( pInstance->bMinimized == false )
        {
            SDL_RenderPresent( pInstance->pRenderer );
        }
    }
}

bool CWindow::IsAllWindowClosed(void)
{
    return ( CWindow::vecInstance.size() == 0 ) ? true : false;
}
