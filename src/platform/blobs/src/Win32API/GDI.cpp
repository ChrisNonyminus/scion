#include "GDI.h"


HBRUSH CreateSolidBrush(COLORREF color) {
  HBRUSH_SDL *brush = new HBRUSH_SDL(color);
  HANDLE handle = HANDLE_Create(Handle_Brush, brush);
  return handle;
}
bool GetMonitorInfoA(int32_t hMonitor, LPMONITORINFO lpmi) {
  //assert(lpmi->cbSize == sizeof(MONITORINFO));
  SDL_Rect rect;
  SDL_GetDisplayBounds(hMonitor, &rect);
  lpmi->rcMonitor.left = rect.x;
  lpmi->rcMonitor.top = rect.y;
  lpmi->rcMonitor.right = rect.x + rect.w;
  lpmi->rcMonitor.bottom = rect.y + rect.h;
  SDL_GetDisplayUsableBounds(hMonitor, &rect);
  lpmi->rcWork.left = rect.x;
  lpmi->rcWork.top = rect.y;
  lpmi->rcWork.right = rect.x + rect.w;
  lpmi->rcWork.bottom = rect.y + rect.h;
  lpmi->dwFlags = hMonitor == 0 ? 1 : 0;
  if (lpmi->cbSize == sizeof(MONITORINFO))
    sprintf(lpmi->name, "Disp#%08x", hMonitor);

  return true;
}
bool CreateDCA() {
  return true;
}
bool GetDeviceCaps() {
  return true;
}
bool DeleteDC() {
  return true;
}
int32_t MonitorFromWindow(SDL_Window *hwnd, uint32_t dwFlags) {
  int32_t displayIndex = SDL_GetWindowDisplayIndex(hwnd);
  if (displayIndex < 0) {
    return 0;
  }
  return displayIndex + 1;
}

/*The InvalidateRect function adds a rectangle to the specified window's update
 * region. The update region represents the portion of the window's client area
 * that must be redrawn.*/
bool InvalidateRect(SDL_Window *hwnd, const RECT *lpRect, bool bErase) {
  //SDL_UpdateWindowSurface(hwnd);
  return true;
}
