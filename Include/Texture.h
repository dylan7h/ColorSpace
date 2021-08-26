#ifndef __TEXTURE__H__
#define __TEXTURE__H__

#include "Window.h"

class CTexture
{
    private:
        CWindow*        pWindowInstance;
        SDL_Texture*    pTexture;
        int             nWidth;
        int             nHeight;

    public:
        CTexture( CWindow* pWindowInstance );
        ~CTexture();

    public:
        void CreateTexture( SDL_PixelFormatEnum ePixelFormat, int nWidth, int nHeight );
        void UpdateTextureData( const void* pPixels, int nPitch, const SDL_Rect* prtClip = nullptr ) const;
        void Render( int x, int y, SDL_Rect* prtClip = nullptr, double dAngle = 0.0, SDL_Point* pptCenter = nullptr, SDL_RendererFlip eFlip = SDL_FLIP_NONE ); 
        void Release(void);

    public:
        void GetScreenRect( SDL_Rect* prtScreen ) const;

    public:
        void LoadFromFile( const char lpszPath[] );
};

#endif  //!__TEXTURE__H__
