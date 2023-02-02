#include <cstring>
#include <csignal>
#include "Misc.h"
DWORD GetTickCount() {
  struct timespec ts;
  clock_gettime(CLOCK_BOOTTIME, &ts);
  return ts.tv_sec * 1000;
}
DWORD timeGetTime() {
  return GetTickCount();
}
bool GetUserNameA(char *lpBuffer, uint32_t *pcbBuffer) {
  getlogin_r(lpBuffer, *pcbBuffer);
  *pcbBuffer = strlen(lpBuffer);
  return true;
}
