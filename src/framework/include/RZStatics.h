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

#pragma once

#include <stdint.h>

#ifdef USING_SDL2

#include "include/SDL.h"

#endif

class cIGZFramework;

class cIGZString;

class cRZCOMDllDirector;

class cIGZWinMgr;

class cIGZCOM;

extern cIGZFramework *RZGetFramework();

extern cRZCOMDllDirector *RZGetCOMDllDirector();
//extern void RZGetCurrentAppPath(cIGZString& output);

extern bool RZIsKeyDownNow(uint32_t key);

extern cIGZCOM *GZCOM();

#if defined(USING_SDL2) && defined(USING_OPENGL)
typedef SDL_GLContext RZGLContext;
typedef SDL_Window* RZWindow;
typedef SDL_Point RZPoint;
#elif defined(_WIN32)
typedef HGLRC RZGLContext;
typedef HWND RZWindow;
typedef POINT RZPoint;
#endif

namespace Display {
    extern bool sInitialized;
    extern int16_t sCurrentWidth;
    extern int16_t sCurrentHeight;
    extern float sXScale;
    extern float sYScale;
    extern bool sInWindowMode;
    extern bool sEnableSwap;
    extern int sSwapCount;
    extern RZGLContext sContextRef;
#if defined(USING_SDL2)
    extern SDL_Window *sMainWindow;
    extern SDL_Rect sDisplayRect;
#elif defined(_WIN32)
    extern HWND sMainWindow;
    extern RECT sDisplayRect;
#endif

    bool SetupDisplay(bool bWindowed, int16_t iWidth, int16_t iHeight);

    int SwapContext(RZGLContext context);

    bool Initialize();

    void ReleaseDisplay();

    RZWindow GetMainWindow();

    void GetDisplayScale(float& xScale, float& yScale);

    RZPoint PtInDisplay(RZPoint pt);

    RZGLContext CreateContext(int a1, uint8_t& a2);

    void ReleaseContext(RZGLContext context);



}

// helper functions for getting commonly used system service interfaces
namespace RZ {
    cIGZWinMgr *WM();
}   // namespace RZ