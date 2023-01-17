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

#ifdef USING_SDL2
#include "include/SDL.h"
#endif

#include "GZWin.h"
#include "cRZSysServPtr.h"

cIGZFramework* RZGetFramework()
{
	return cGZFramework::mpFramework;
}

cRZCOMDllDirector* RZGetCOMDllDirector()
{
	static cGZCOMDirector gDirector;
	return &gDirector;
}

bool RZIsKeyDownNow(uint32_t key)
{
#ifdef USING_SDL2
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == key)
            {
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

cIGZWinMgr *RZ::WM() {
    static cRZSysServPtr<cIGZWinMgr, cIGZWinMgr::CLSID, cIGZWinMgr::ServiceID> spWM;
    return spWM;
}


/*cIGZFontSys *RZ::FO() {
    return nullptr;
}*/
