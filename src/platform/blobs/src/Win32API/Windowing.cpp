#define GLAD_GL_IMPLEMENTATION
#include <map>
#include <vector>
#include "Windowing.h"

extern SDL_Window* gMainHWND;

extern "C" uint32_t OnPaintCallback(uint32_t interval, void* param) {;
  if (GetWindowLongA(gMainHWND, -21) == NULL)
    return 16;
  if (gMainHWND == NULL)
    return 16;
  /*__ZN11nGZGraphic410cCanvasW327OnPaintEP6HWND__(GetWindowLongA
                                                     (gMainHWND, -21), gMainHWND);*/
  MSG msg;
  msg.hwnd = gMainHWND;
  msg.message = 15;
  DispatchMessageA(&msg);
  return 16;
}

void* WindowCallbackRoutine(void*) {
  static uint64_t msLastOnPaint = time(NULL) * 1000;
  uint64_t msNow = time(NULL) * 1000;
  while (true) {
    msNow = time(NULL) * 1000;
    uint64_t interval = (msNow - msLastOnPaint);
    if (interval >= 16 ) {
      OnPaintCallback(0, NULL);
      msNow = msLastOnPaint;
      continue;
    }

    if (gMainHWND == NULL) {
      break;
    }
  }
}

pthread_t windowThread;

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
  //SDL_AddTimer(16, OnPaintCallback, NULL);
  //pthread_create(&windowThread, NULL, WindowCallbackRoutine, NULL);
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
  SendMessageA(wind, 15, 0, 0);
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

std::vector<MSG> sMsgQueue;

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

  if (sMsgQueue.size() > 0) {
    *lpMsg = sMsgQueue[0];
    sMsgQueue.erase(sMsgQueue.begin());
    return sMsgQueue.size() > 0;
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
int SendMessageA(SDL_Window *hWnd,
                 uint32_t Msg,
                 uintptr_t wParam,
                 long lParam) {
  MSG msg;
  msg.hwnd = hWnd;
  msg.message = Msg;
  msg.wParam = wParam;
  msg.lParam = lParam;

  return DispatchMessageA(&msg);
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
bool PostMessageA(
    /*in, optional*/ SDL_Window* hWnd,
    /*in*/ uint32_t Msg,
    /*in*/ uintptr_t wParam,
    /*in*/ long lParam
) {
  MSG msg;
  msg.hwnd = hWnd;
  msg.message = Msg;
  msg.wParam = wParam;
  msg.lParam = lParam;
  sMsgQueue.push_back(msg);
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
