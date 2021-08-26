#include <assert.h>
#include <SDL_image.h>
#include "Texture.h"

CTexture::CTexture( CWindow* pWindowInstance )
{
    assert( pWindowInstance != nullptr );

    this->pWindowInstance = pWindowInstance;
    this->pTexture = nullptr;
}

CTexture::~CTexture()
{
    this->Release();
}

void CTexture::CreateTexture( SDL_PixelFormatEnum ePixelFormat, int nWidth, int nHeight )
{
    assert( this->pTexture == nullptr );

    this->pTexture = SDL_CreateTexture( this->pWindowInstance->pRenderer, ePixelFormat, SDL_TEXTUREACCESS_STREAMING, nWidth, nHeight );
    assert( this->pTexture != nullptr );

    this->nWidth = nWidth;
    this->nHeight = nHeight;
}

void CTexture::UpdateTextureData( const void* pPixels, int nPitch, const SDL_Rect* prtClip ) const
{
    assert( this->pTexture != nullptr );
    SDL_UpdateTexture( this->pTexture, prtClip, pPixels, nPitch );
}

void CTexture::Render( int x, int y, SDL_Rect* prtClip, double dAngle, SDL_Point* pptCenter, SDL_RendererFlip eFlip )
{
    SDL_Rect renderQuad = { x, y, this->nWidth, this->nHeight };

    assert( this->pTexture != nullptr );
    if( prtClip != nullptr )
    {
        renderQuad.w = prtClip->w;
        renderQuad.h = prtClip->h;
    }

    SDL_RenderCopyEx( this->pWindowInstance->pRenderer, this->pTexture, prtClip, &renderQuad, dAngle, pptCenter, eFlip );
}

void CTexture::Release(void)
{
    if( this->pTexture != nullptr )
    {
        SDL_DestroyTexture( this->pTexture );

        this->pWindowInstance = nullptr;
        this->pTexture = nullptr;
    }
}

void CTexture::GetScreenRect( SDL_Rect* prtScreen ) const
{
    assert( prtScreen != nullptr );

    prtScreen->x = 0;
    prtScreen->y = 0;
    prtScreen->w = this->pWindowInstance->nWidth;
    prtScreen->h = this->pWindowInstance->nHeight;
}

void CTexture::LoadFromFile( const char lpszPath[] )
{
    SDL_Surface* pLoadedSurface;
    assert( this->pTexture == nullptr );

    pLoadedSurface = IMG_Load( lpszPath );
    assert( pLoadedSurface != nullptr );

    this->pTexture = SDL_CreateTextureFromSurface( this->pWindowInstance->pRenderer, pLoadedSurface );
    assert( this->pTexture != nullptr );

    this->nWidth    = pLoadedSurface->w;
    this->nHeight   = pLoadedSurface->h;

    SDL_FreeSurface( pLoadedSurface );
}
