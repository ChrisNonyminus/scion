#define GLAD_GL_IMPLEMENTATION
#include <map>
#include "Windowing.h"

extern SDL_Window* gMainHWND;

extern "C" uint32_t OnPaintCallback(uint32_t interval, void* param) {;
  __ZN11nGZGraphic410cCanvasW327OnPaintEP6HWND__(GetWindowLongA
                                                     (GetActiveWindow(), -21), GetActiveWindow());
  return 16;
}

void *CreateWindowExA(uint32_t dwExStyle,
                      const char *lpClassName,
                      const char *lpWindowName,
                      uint32_t dwStyle,
                      int X,
                      int Y,
                      int nWidth,
                      int nHeight,
                      HWND hWndParent,
                      HANDLE hMenu,
                      HANDLE hInstance,
                      void *lpParam) {
  // TODO: The mac version seems to be calling CreateWindowExA wrong and
  //  giving the window a position, width and height of INT_MIN.
  SDL_Window *mSDLWindow = SDL_CreateWindow(lpWindowName != NULL ?
                                            lpWindowName :
                                            "Scion", SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED, 1024,
                                            768,
                                            SDL_WINDOW_VULKAN| SDL_WINDOW_RESIZABLE);

  SDL_HideWindow(mSDLWindow);
  gMainHWND = mSDLWindow;
  SDL_AddTimer(16, OnPaintCallback, NULL);
  return mSDLWindow;

}
bool ShowWindow_Win32(HWND hWnd, int nCmdShow) {
  SDL_Window *wind;
  if (hWnd != NULL)
    wind = (SDL_Window *) hWnd;
  if (wind == NULL)
    return false;
  bool windowPreviouslyVisible = (SDL_GetWindowFlags(wind) &
      SDL_WINDOW_SHOWN) != 0;
  switch (nCmdShow) {
  case 0:SDL_HideWindow(wind);
    break;
  case 1:
  case 5:SDL_ShowWindow(wind);
    break;
  default:break;
  }
  return windowPreviouslyVisible;
}

bool UpdateWindow(
    /*in*/ HWND hWnd
) {
  SDL_Window *wind;
  if (hWnd != NULL)
    wind = (SDL_Window *) hWnd;
  if (wind == NULL)
    return false;
  //SDL_UpdateWindowSurface(wind);

  //SDL_GL_SwapWindow(wind);
  return true;
}
bool GetClientRect(HWND hWnd, LPRECT lpRect) {
  SDL_Window *wind;
  if (hWnd != NULL)
    wind = (SDL_Window *) hWnd;
  if (wind == NULL)
    return false;
  SDL_GetWindowPosition(wind, &lpRect->left, &lpRect->top);
  SDL_GetWindowSize(wind, &lpRect->right, &lpRect->bottom);
  lpRect->right += lpRect->left;
  lpRect->bottom += lpRect->top;
  return true;

}
bool ClientToScreen(HWND hWnd, LPPOINT lpPoint) {
  SDL_Window *wind;
  if (hWnd != NULL)
    wind = (SDL_Window *) hWnd;
  if (wind == NULL)
    return false;
  int x, y;
  SDL_GetWindowPosition(wind, &x, &y);
  lpPoint->x -= x;
  lpPoint->y -= y;
  return true;
}
SDL_Cursor *LoadCursorA(HANDLE hInstance, const char *lpCursorName) {
  SDL_Cursor *cursor = NULL;
  // TODO
  cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  SDL_SetCursor(cursor);
  return cursor;
}

#define WM_QUIT                         0x0012

#define WM_KEYFIRST                     0x0100
#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101

#define WM_MOUSEFIRST                   0x0200
#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209
#define WM_MOUSEWHEEL                   0x020A
bool PeekMessageA(LPMSG lpMsg,
                  SDL_Window *hWnd,
                  uint32_t wMsgFilterMin,
                  uint32_t wMsgFilterMax,
                  uint32_t wRemoveMsg) {
  lpMsg->hwnd = hWnd;

  static bool wereIn = false;
  if (!wereIn) {
    // *hacker voice* we're in
    lpMsg->message = 0x1C;
    lpMsg->wParam = 1;
    lpMsg->lParam = pthread_self();
    wereIn = true;
    return true;
  }
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT: {
      lpMsg->message = WM_QUIT;
      lpMsg->wParam = 0;
      return true;
    }
    case SDL_KEYDOWN: {
      lpMsg->message = WM_KEYDOWN;
      lpMsg->wParam = event.key.keysym.scancode;
      return false;
    }
    case SDL_KEYUP: {
      lpMsg->message = WM_KEYUP;
      lpMsg->wParam = event.key.keysym.scancode;
      return false;
    }
    case SDL_MOUSEMOTION: {
      lpMsg->message = WM_MOUSEMOVE;
      lpMsg->wParam = event.motion.state;
      int x, y;
      SDL_GetMouseState(&x, &y);
      lpMsg->pt.x = x;
      lpMsg->pt.y = y;
      return false;
    }
    case SDL_MOUSEBUTTONDOWN: {
      lpMsg->message = WM_LBUTTONDOWN;
      lpMsg->wParam = event.button.button;
      int x, y;
      SDL_GetMouseState(&x, &y);
      lpMsg->pt.x = x;
      lpMsg->pt.y = y;
      return false;
    }
    case SDL_MOUSEBUTTONUP: {
      lpMsg->message = WM_LBUTTONUP;
      lpMsg->wParam = event.button.button;
      int x, y;
      SDL_GetMouseState(&x, &y);
      lpMsg->pt.x = x;
      lpMsg->pt.y = y;
      return false;
    }
    case SDL_MOUSEWHEEL: {
      lpMsg->message = WM_MOUSEWHEEL;
      lpMsg->wParam = event.wheel.y;
      lpMsg->lParam = event.wheel.x;
      int x, y;
      SDL_GetMouseState(&x, &y);
      lpMsg->pt.x = x;
      lpMsg->pt.y = y;
      return false;
    }
    default:break;
    }
    return false;
  }
  return SDL_PeepEvents(NULL, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT)
  > 0;
}
bool PostMessageA(
    /*in, optional*/ SDL_Window* hWnd,
    /*in*/ uint32_t Msg,
    /*in*/ uintptr_t wParam,
    /*in*/ long lParam
) {
  SDL_Event event;
  switch (Msg) {
  case WM_QUIT: {
    event.type = SDL_QUIT;
    break;
  }
  case WM_KEYDOWN: {
    event.type = SDL_KEYDOWN;
    event.key.keysym.scancode = static_cast<SDL_Scancode>(wParam);
    break;
  }
  case WM_KEYUP: {
    event.type = SDL_KEYUP;
    event.key.keysym.scancode = static_cast<SDL_Scancode>(wParam);
    break;
  }
  case WM_MOUSEMOVE: {
    event.type = SDL_MOUSEMOTION;
    event.motion.state = wParam;
    event.motion.x = lParam & 0xFFFF;
    event.motion.y = lParam >> 16;
    break;
  }
  case WM_LBUTTONDOWN: {
    event.type = SDL_MOUSEBUTTONDOWN;
    event.button.button = wParam;
    event.button.x = lParam & 0xFFFF;
    event.button.y = lParam >> 16;
    break;
  }
  case WM_LBUTTONUP: {
    event.type = SDL_MOUSEBUTTONUP;
    event.button.button = wParam;
    event.button.x = lParam & 0xFFFF;
    event.button.y = lParam >> 16;
    break;
  }
  case WM_MOUSEWHEEL: {
    event.type = SDL_MOUSEWHEEL;
    event.wheel.y = wParam;
    event.wheel.x = lParam;
    break;
  }
  default: printf("WARNING! Unknown message type %08X, wParam %d, lParam "
                  "%d\n",Msg, wParam, lParam); return false;
  }
  SDL_PushEvent(&event);
  return true;
}
static SDL_Window* activeWindow;
SDL_Window *SetActiveWindow(SDL_Window *hwnd) {
  SDL_Window* prev = activeWindow;
  activeWindow = hwnd;
  return prev;
}
SDL_Cursor *LoadCursorFromFileA(const char *lpFileName) {
  SDL_Cursor *cursor = NULL;
  // TODO
  cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  SDL_SetCursor(cursor);
  return cursor;
}
bool DestroyCursor(SDL_Cursor *cursor) {
  SDL_FreeCursor(cursor);
  return true;
}
bool DispatchMessageA(LPMSG lpMsg) {
  if (!lpMsg) return false;
  if (!lpMsg->hwnd) lpMsg->hwnd = gMainHWND;
  if (lpMsg->hwnd) {
    return __ZN11nGZGraphic410cCanvasW3210WindowProcEP6HWND__jjl(lpMsg->hwnd,
                                                          lpMsg->message,
                                                          lpMsg->wParam,
                                                          lpMsg->lParam) != 0;
  }
  return false;
}

std::map<int, void*> longValues;

void *GetWindowLongA(SDL_Window * hwnd, int idx) {
  if (!longValues.count(idx)) return NULL;
  return longValues[idx];
}
void SetWindowLongA(SDL_Window *hwnd, int idx, void *lval) {
  longValues.insert(std::pair<int, void*>(idx, lval));
}
SDL_Window *GetActiveWindow() {
  return activeWindow;
}
bool SetWindowTextA(SDL_Window *hwnd, const char *str) {
  SDL_SetWindowTitle(hwnd, str);
  return true;
}

