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

#include <cstring>
#include "cRZKeyboard.h"
#include "RZStatics.h"

#ifdef USING_SDL2
#include <SDL.h>
#endif

cRZKeyboard::cRZKeyboard()
{
	ClearKeyboard();
}

void cRZKeyboard::ClearKeyboard()
{
	memset(keyStates, 0, sizeof(keyStates));
	keysPressed.clear();
}

bool cRZKeyboard::IsKeyDown(uint32_t key)
{
	return keyStates[key];
}

bool cRZKeyboard::IsKeyDown(uint32_t key, uint32_t modifiers)
{
	if (!IsKeyDown(key))
	{
		return false;
	}
	else
	{
		return GetCurrentModifierState() == modifiers;
	}
}

bool cRZKeyboard::IsKeyDownNow(uint32_t key)
{
	return RZIsKeyDownNow(key);
}

bool cRZKeyboard::CharIsModifier(uint32_t key)
{
	/*switch (key)
	{
	case VK_SHIFT:
	case VK_CONTROL:
	case VK_MENU:
		return true;

	default:
		return false;
	}*/
#ifdef USING_SDL2
    switch (key)
    {
    case SDLK_LSHIFT:
    case SDLK_RSHIFT:
    case SDLK_LCTRL:
    case SDLK_RCTRL:
    case SDLK_LALT:
    case SDLK_RALT:
        return true;

    default:
        return false;
    }
#endif
}

uint32_t cRZKeyboard::GetCurrentModifierState()
{
	/*uint32_t modifiers = 0;

	if (IsKeyDown(VK_SHIFT))   modifiers |= kShiftModifier;
	if (IsKeyDown(VK_CONTROL)) modifiers |= kControlModifier;
	if (IsKeyDown(VK_MENU))    modifiers |= kMenuModifier;

	return modifiers;*/

    uint32_t modifiers = 0;

#ifdef USING_SDL2
    if (IsKeyDown(SDLK_LSHIFT) || IsKeyDown(SDLK_RSHIFT)) modifiers |= kShiftModifier;
    if (IsKeyDown(SDLK_LCTRL) || IsKeyDown(SDLK_RCTRL)) modifiers |= kControlModifier;
    if (IsKeyDown(SDLK_LALT) || IsKeyDown(SDLK_RALT)) modifiers |= kMenuModifier;
#endif

        return modifiers;
}

bool cRZKeyboard::IsToggleKeySet(uint32_t key)
{
	return IsToggleKeySetStatic(key);
}

void cRZKeyboard::ProcessKeyDown(uint32_t key)
{
	if (!keyStates[key])
	{
		keyStates[key] = true;
		keysPressed.push_back(key);
	}
}

void cRZKeyboard::ProcessKeyUp(uint32_t key)
{
	if (keyStates[key])
	{
		keyStates[key] = false;
		keysPressed.remove(key);
	}
}

bool cRZKeyboard::IsKeyDownNowStatic(uint32_t key)
{
	return RZIsKeyDownNow(key);
}

bool cRZKeyboard::IsToggleKeySetStatic(uint32_t key)
{
	//return (GetKeyState(key) & 1) != 0;
#if defined(USING_SDL2)
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
#endif
    return false;
}