/*
 *  Scion - an open-source implementation of the Maxis GZCOM/RZCOM framework
 *  Copyright (C) 2021  Nelson Gomez (nsgomez) <nelson@ngomez.me>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation, under
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "cGZCOMDirector.h"
#include "cGZFramework.h"
#include "RZStatics.h"
#include "cRZExceptionNotification.h"
#include "cRZString.h"

#ifdef USING_SDL2

#include "include/SDL.h"

#endif

#include "GZWin.h"
#include "cRZSysServPtr.h"

cIGZFramework *RZGetFramework() {
    return cGZFramework::mpFramework;
}

cRZCOMDllDirector *RZGetCOMDllDirector() {
    static cGZCOMDirector gDirector;
    return &gDirector;
}

bool RZIsKeyDownNow(uint32_t key) {
#ifdef USING_SDL2
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == key) {
                return true;
            }
        }
    }
    return false;
#elif defined(_WIN32)
    if (key == VK_LBUTTON && GetSystemMetrics(SM_SWAPBUTTON) != 0)
    {
        key = VK_RBUTTON;
    }

    return (GetAsyncKeyState(key) >> 15) != 0;
#endif
}

cIGZCOM *GZCOM() {
    return RZGetCOMDllDirector()->GZCOM();
}

namespace Display {
    bool sInitialized;
    int16_t sCurrentWidth;
    int16_t sCurrentHeight;
    float sXScale;
    float sYScale;
    bool sInWindowMode;
    bool sEnableSwap;
    int sSwapCount;
    RZGLContext sContextRef;
#if defined(USING_SDL2)
    SDL_Window *sMainWindow;
    SDL_Rect sDisplayRect;
#elif defined(_WIN32)
    HWND sMainWindow;
    RECT sDisplayRect;
#endif

    bool SetupDisplay(bool bWindowed, int16_t iWidth, int16_t iHeight) {
        if (!sInitialized) {
            return true;
        }

        sInWindowMode = bWindowed;
        sCurrentWidth = iWidth;
        sCurrentHeight = iHeight;
        sXScale = (float) iWidth / 1024.0f; // TODO: get target width (1024) from sDisplayRect
        sYScale = (float) iHeight / 768.0f; // TODO: get target height (768) from sDisplayRect
        if (bWindowed) {
#if defined(USING_SDL2)
            int windowFlags = SDL_WINDOW_SHOWN;
            if (iWidth == 0 || iHeight == 0) {
                windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
            } else {
                windowFlags |= SDL_WINDOW_RESIZABLE;
            }
#if defined(USING_OPENGL)
            windowFlags |= SDL_WINDOW_OPENGL;
#endif

            sMainWindow = SDL_CreateWindow(SZ_DEFAULT_APP_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, iWidth,
                                           iHeight, windowFlags);
            if (sMainWindow == nullptr) {
                return false;
            }

#elif defined(_WIN32)
            DWORD windowStyle = WS_OVERLAPPEDWINDOW;
            if (iWidth == 0 || iHeight == 0) {
                windowStyle = WS_POPUP;
            }

            sMainWindow = CreateWindow(SZ_DEFAULT_APP_TITLE, SZ_DEFAULT_APP_TITLE, windowStyle, CW_USEDEFAULT,
                                       CW_USEDEFAULT, iWidth, iHeight, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
            if (sMainWindow == nullptr) {
                return false;
            }

            if (iWidth == 0 || iHeight == 0) {
                SetWindowLong(sMainWindow, GWL_STYLE, WS_POPUP);
                SetWindowPos(sMainWindow, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                             SWP_SHOWWINDOW);
            }
#endif
        } else {
#if defined(USING_SDL2)
            int windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
#if defined(USING_OPENGL)
            windowFlags |= SDL_WINDOW_OPENGL;
#endif
            sMainWindow = SDL_CreateWindow(SZ_DEFAULT_APP_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, iWidth,
                                           iHeight, windowFlags);
            if (sMainWindow == nullptr) {
                return false;
            }
#elif defined(_WIN32)
            sMainWindow = CreateWindow(SZ_DEFAULT_APP_TITLE, SZ_DEFAULT_APP_TITLE, WS_POPUP, 0, 0, iWidth, iHeight, nullptr,
                                       nullptr, GetModuleHandle(nullptr), nullptr);
            if (sMainWindow == nullptr) {
                return false;
            }
#endif
        }
        return true;
    }

    int SwapContext(RZGLContext context) {
        if (!sEnableSwap) {
            return -1;
        }

#if defined(USING_SDL2)
        SDL_GL_SwapWindow(sMainWindow);
#elif defined(_WIN32)
        SwapBuffers(context);
#endif

        ++sSwapCount;
        return 0;
    }

    bool Initialize() {
        if (sInitialized) {
            return true;
        }

#if defined(USING_SDL2)
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
            cRZString sError = "Failed to initialize SDL video subsystem: ";
            sError += SDL_GetError();
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", sError.c_str(), nullptr);

            return false;
        }

        SDL_DisplayMode displayMode;
        if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
            cRZString sError = "Failed to get desktop display mode: ";
            sError += SDL_GetError();
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", sError.c_str(), nullptr);
            return false;
        }

        sDisplayRect.x = 0;
        sDisplayRect.y = 0;
        sDisplayRect.w = displayMode.w;
        sDisplayRect.h = displayMode.h;

        sCurrentWidth = sDisplayRect.w;
        sCurrentHeight = sDisplayRect.h;
        sXScale = (float) sCurrentWidth / 1024.0f;
        sYScale = (float) sCurrentHeight / 768.0f;
#elif defined(_WIN32)
        sDisplayRect.left = 0;
        sDisplayRect.top = 0;
        sDisplayRect.right = GetSystemMetrics(SM_CXSCREEN);
        sDisplayRect.bottom = GetSystemMetrics(SM_CYSCREEN);

        sCurrentWidth = sDisplayRect.right;
        sCurrentHeight = sDisplayRect.bottom;
        sXScale = (float) sCurrentWidth / 1024.0f;
        sYScale = (float) sCurrentHeight / 768.0f;
#endif

        sInitialized = true;
        return true;


    }

    void ReleaseDisplay() {
        if (!sInitialized) {
            return;
        }

#if defined(USING_SDL2)
        SDL_DestroyWindow(sMainWindow);
#elif defined(_WIN32)
        DestroyWindow(sMainWindow);
#endif

        sMainWindow = nullptr;
    }

    RZWindow GetMainWindow() {
        return sMainWindow;
    }

    void GetDisplayScale(float &xScale, float &yScale) {
        xScale = sXScale;
        yScale = sYScale;
    }

    RZPoint PtInDisplay(RZPoint pt) {
        pt.x = (int) (pt.x * sXScale);
        pt.y = (int) (pt.y * sYScale);
        return pt;
    }

    RZGLContext CreateContext(int a1, uint8_t &a2) {
#if defined(USING_SDL2)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

        SDL_GLContext context = SDL_GL_CreateContext(sMainWindow);
        if (context == nullptr) {
            return nullptr;
        }

        SDL_GL_MakeCurrent(sMainWindow, context);

        sContextRef = context;

        return context;
#elif defined(_WIN32)

#error "Not implemented"

#endif

    }

    void ReleaseContext(RZGLContext context) {
#if defined(USING_SDL2)
        SDL_GL_DeleteContext(context);
#elif defined(_WIN32)

#error "Not implemented"

#endif

    }
}

cIGZWinMgr *RZ::WM() {
    static cRZSysServPtr<cIGZWinMgr, GZIID_cIGZWinMgr, RZSRVID_cGZWinMgr> spWM;
    return spWM;
}


/*cIGZFontSys *RZ::FO() {
    return nullptr;
}*/
