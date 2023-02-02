#ifndef BLOBS_SRC_WIN32API_GDI_H
#define BLOBS_SRC_WIN32API_GDI_H

#include "WinUtils.h"
#include "Handle.h"

#include "Windowing.h"
#include <SDL2/SDL.h>

typedef DWORD COLORREF; // 0x00bbggrr
#define GET_R_VALUE(rgb) ((uint8_t)(rgb))
#define GET_G_VALUE(rgb) ((uint8_t)(((uint16_t)(rgb)) >> 8))
#define GET_B_VALUE(rgb) ((uint8_t)((rgb)>>16))
typedef HANDLE HBRUSH;

extern "C" HBRUSH CreateSolidBrush(
    /*in*/ COLORREF color
);

class HBRUSH_SDL {
public:
  HBRUSH_SDL(COLORREF color) {
    m_color = color;
  }
  ~HBRUSH_SDL() {
  }

  COLORREF m_color;
};

typedef struct tagMONITORINFO {
  DWORD cbSize;
  RECT rcMonitor;
  RECT rcWork;
  DWORD dwFlags;
  char name[32];
} MONITORINFO, *LPMONITORINFO;

bool GetMonitorInfoA(
    /*in*/ int32_t hMonitor, // index of the monitor/display
    /*out*/ LPMONITORINFO lpmi
);

extern "C" bool CreateDCA(); // STUB
extern "C" bool GetDeviceCaps(); // STUB
extern "C" bool DeleteDC(); // STUB

int32_t MonitorFromWindow(
    /*in*/ SDL_Window *hwnd, // A handle to the window of interest.
    /*in*/
           DWORD dwFlags // Determines the function's return value if the window does not intersect any display monitor.
);

bool InvalidateRect(
    /*in*/ SDL_Window *hwnd,
    /*in*/ const RECT *lpRect,
    /*in*/ bool bErase
);

#endif //BLOBS_SRC_WIN32API_GDI_H
