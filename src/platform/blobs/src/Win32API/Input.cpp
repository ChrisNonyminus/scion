#include <stdint.h>
#include "Input.h"
short GetKeyState(int nVirtKey) {
  switch (nVirtKey) {
  case 1: /*VK_LBUTTON*/ {
    uint32_t mouseState = SDL_GetMouseState(NULL, NULL);
    if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
      return -32768;
    return 0;
  }
  case 2: /*VK_RBUTTON*/ {
    uint32_t mouseState = SDL_GetMouseState(NULL, NULL);
    if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
      return -32768;
    return 0;
  }
  case 4: /*VK_MBUTTON*/ {
    uint32_t mouseState = SDL_GetMouseState(NULL, NULL);
    if (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE))
      return -32768;
    return 0;
  }
  }
  const uint8_t *state = SDL_GetKeyboardState(NULL);
  if (state[nVirtKey] == 1)
    return -32768;
  return 0;
}
