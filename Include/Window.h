#ifndef __WINDOW__H__
#define __WINDOW__H__

#include <SDL.h>
#include <vector>

class CWindow
{
    private:
        friend class CTexture;

    private:
        SDL_Window*     pWindow;
        SDL_Renderer*   pRenderer;

        int             nWidth;
        int             nHeight;

        bool            bMouseFocus;
		bool            bKeyboardFocus;
		bool            bFullScreen;
		bool            bMinimized;
		bool            bShown;

        int             nWindowID;

    private:
        static std::vector<CWindow*> vecInstance;

    public:
        CWindow();
        ~CWindow();
    
    public:
        static void SDL_StartUp(void);
        static void SDL_Release(void);

    public:
        void Initialize(const char lpszTitle[], int nWidth, int nHeight);
        void DeInitialize(void);

    public:
        static void UpdateWindowState( SDL_Event& e );
        static void ClearScreen(void);
        static void RenderPresent(void);
        static bool IsAllWindowClosed(void);
};

#endif  //!__WINDOW__H__
