#ifndef BLOBS_SRC_WIN32API_WINDOWING_H
#define BLOBS_SRC_WIN32API_WINDOWING_H

#include "WinUtils.h"
#include "Handle.h"

#include "glad/gl.h"
#include "glad/vulkan.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_vulkan.h>

typedef HANDLE HWND; // HWND will be a pointer to SDL_Window.

void *CreateWindowExA(
    /*in*/ DWORD dwExStyle,
    /*in, optional*/ const char *lpClassName,
    /*in, optional*/ const char *lpWindowName,
    /*in*/ DWORD dwStyle,
    /*in*/ int X,
    /*in*/ int Y,
    /*in*/ int nWidth,
    /*in*/ int nHeight,
    /*in, optional*/ HWND hWndParent,
    /*in, optional*/ HANDLE hMenu,
    /*in, optional*/ HANDLE hInstance, // obtained through GetModuleHandleA
    /*in, optional*/ void *lpParam
);

bool ShowWindow_Win32(
    /*in*/ HWND hWnd,
    /*in*/ int nCmdShow
);

bool UpdateWindow(
    /*in*/ HWND hWnd
);

typedef struct tagRECT {
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;
typedef struct tagPOINT {
  long x;
  long y;
} POINT, *PPOINT, *NPPOINT, *LPPOINT;

bool GetClientRect(
    /*in*/ HWND hWnd,
    /*out*/ LPRECT lpRect
);

bool ClientToScreen(
    /*in*/ HWND hWnd,
    /*in, out*/ LPPOINT lpPoint
);

SDL_Cursor *LoadCursorA(
    /*in, optional*/ HANDLE hInstance,
    /*in*/ const char *lpCursorName
);

SDL_Cursor *LoadCursorFromFileA(
    /*in*/ const char* lpFileName
    );

bool DestroyCursor(SDL_Cursor* cursor);

typedef struct tagMSG {
  SDL_Window* hwnd;
  uint32_t message;
  uintptr_t wParam;
  long lParam;
  DWORD time;
  POINT pt;
  uint8_t field_1C, field_1D, field_1E;
} MSG, *PMSG, *NPMSG, *LPMSG;

bool PeekMessageA(
    /*out*/ LPMSG lpMsg,
    /*in, optional*/ SDL_Window *hWnd,
    /*in*/ uint32_t wMsgFilterMin,
    /*in*/ uint32_t wMsgFilterMax,
    /*in*/ uint32_t wRemoveMsg
);

bool PostMessageA(
    /*in, optional*/ SDL_Window* hWnd,
    /*in*/ uint32_t Msg,
    /*in*/ uintptr_t wParam,
    /*in*/ long lParam
    );

bool DispatchMessageA(
    /*in*/ LPMSG lpMsg
    );

void* GetWindowLongA(SDL_Window* hwnd, int idx);
void SetWindowLongA(SDL_Window* hwnd, int idx, void* lval);

SDL_Window *GetActiveWindow();

SDL_Window *SetActiveWindow(SDL_Window* hwnd);

bool SetWindowTextA(SDL_Window* hwnd, const char* str);

// Temporary
extern int (*__ZN11nGZGraphic410cCanvasW3210WindowProcEP6HWND__jjl)
(SDL_Window* hWnd, uint32_t uMsg, uint32_t wParam, int32_t lParam);
extern int (*__ZN11nGZGraphic410cCanvasW327OnPaintEP6HWND__)(void* windowlong,
    SDL_Window* hWnd);

#endif //BLOBS_SRC_WIN32API_WINDOWING_H
