#include <string>
#include <dirent.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include "RZHooks.h"
#include "cRZString.h"
#include "cRZVersion.h"
#include "ld-mac.hh"
#include "Win32API/GDI.h"
#ifdef TARGET_GAME_sc4
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimerC1Ev,0x004468F2)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimer13SetResolutionEj, 0x00446930)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimer9SetPeriodEj,0x004469CE)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimer9GetPeriodERj,0x00446A56)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimer13StartPeriodicEjP11cIGZUnknown,0x00446A6C)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimer12StartOneShotEjP11cIGZUnknown,0x00446B40)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimer4StopEv, 0x00446C26)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimerD1Ev,0x00446CBC)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimer19SetCallbackFunctionEPFvjEj, 0x00446D00)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimer19SetCallbackFunctionEPFvPvES0_,0x00446D1A)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimer20EnableSharedTimerUseEb,0x00446D34)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimer31GetCurrentSystemMillisecondTimeEv, 0x00446D3E)
DECL_EXTERN_C_FUNC(_ZN16cRZCallbackTimer9IsRunningEv,0x007A6550)

DECL_EXTERN_C_FUNC(_ZN10cRZVersion12GetUserNameAER9cRZStringPS0_,0x0002AD6E)
DECL_EXTERN_C_FUNC(_ZN10cRZVersion13GetModuleNameER9cRZString,0x0002AF38)
DECL_EXTERN_C_FUNC(_ZN10cRZVersion16GetModuleVersionER9cRZString,0x0002AE5E)
DECL_EXTERN_C_FUNC(_ZN10cRZVersion11GetHostNameER9cRZStringPS0_,0x0002AC84)
DECL_EXTERN_C_FUNC(_ZN10cRZVersion11GetCPUSpeedEv, 0x0002ABAE)
DECL_EXTERN_C_FUNC(_ZN10cRZVersion15GetSystemMemoryERjS0_, 0x0002ABC8)

#define ___ZN22nsGZLua5CompatWithLua47OpenLibEP8cIGZLua5_ADDR 0x0046BF1A

DECL_EXTERN_C_FUNC(_Z14RZFileFindNextP14RZFileFindData,0x0001F09C)
DECL_EXTERN_C_FUNC(_ZN12cRZDirectoryC1ERK9cRZStringbbb,0x000200EE)

#elif TARGET_GAME_sims2
#define \
__ZN16nGZCommandParser11cBaseParser15RegisterCommandEPNS_9cICommandE_ADDR 0x0006CD50
#define \
__ZN16nGZCommandParser11cBaseParser16RegisterFunctionEPNS_20cIExpressionFunctionE_ADDR 0x0006A488
#define __ZN6nGZLog21cGZLogSystemTimestampC2Ev_ADDR 0x001C122A
#define __Z10IsFullPathPKc_ADDR 0x0051035C
#define __ZN9cRZStringC2ERKS_jj_ADDR 0x0050CCA4
#define _GetTimebase_ADDR 0x0000E200


extern "C" void _ZN10cRZVersion16GetModuleVersionER9cRZString();
extern "C" void _ZN10cRZVersion13GetModuleNameER9cRZString();
extern "C" void _ZN10cRZVersion11GetHostNameER9cRZStringPS0_();
extern "C" void _ZN10cRZVersion15GetSystemMemoryERjS0_();
#endif

int (*__ZN11nGZGraphic410cCanvasW3210WindowProcEP6HWND__jjl)
    (SDL_Window* hWnd, uint32_t uMsg, uint32_t wParam, int32_t lParam);

// TODO: tmp stub
int __ZN22nsGZLua5CompatWithLua47OpenLibEP8cIGZLua5() {
  return 0;
}

// TODO: tmp stub
bool _Z10IsFullPathPKc(const char *) {
  return true;
}

extern "C" void _ZN9cRZStringC2ERKS_jj();

// TODO: Temporary hook before I implement the actual class
extern "C" void __ZN11nGZGraphic410cCanvasW325ClearEv(void* this_ptr) {
  SDL_Window* wind = (SDL_Window*)((uint8_t*)this_ptr + 0x68);
  HBRUSH brush = (HBRUSH)((uint8_t*)this_ptr + 0x6C);
  HBRUSH_SDL* realbrush = static_cast<HBRUSH_SDL *>(brush->ptr);
  if (realbrush) {
    SDL_FillRect(SDL_GetWindowSurface(wind), NULL, realbrush->m_color);
  }


}

uint64_t GetTimebase(uint64_t* t) {
  return time(reinterpret_cast<time_t *>(t));
}

int ConvertStringEncoding(void const*, unsigned long, long, void*, unsigned
long&, long);
int ConvertStringEncoding(const void * wcs, unsigned long wlen, long codepage,
                              cIGZString& str) {
  if (codepage == 32) {
    char tmp[1024];
    std::wcstombs(tmp, static_cast<const wchar_t *>(wcs), wlen);
    str.FromChar(tmp);
    return str.Strlen();
  }
  else {
    if (codepage == -1) codepage = 8;
    if (codepage == 8 || codepage == -2) {
      str.FromChar((const char*)(wcs), wlen);
      return wlen;
    }
    unsigned long maxsize = 6 * wlen;
    char buf[512];
    char* dst;
    char* bigdst = NULL;
    if (maxsize >= 512) {
      bigdst = new char[maxsize];
      dst = bigdst;
    } else dst = buf;
    int ret = ConvertStringEncoding(wcs, wlen, codepage, dst, maxsize, 8);
    str.FromChar(dst, maxsize);
    if (bigdst) delete[] bigdst;
    return ret;
  }
}
extern "C" void _Z21ConvertStringEncodingPKvmlR10cIGZString();

extern "C" uint32_t __ZN7cRZTime24GetMillisecondSystemTimeEv() {
  return 1000 * (clock() / CLOCKS_PER_SEC);
}

extern "C" int __ZN15cRZThreadSignal9TimedWaitEm(void* this_ptr, unsigned
long waitlen) {
  // TODO
  return 0;
}

void HookGZFrameFunctions() {
#ifdef TARGET_GAME_sc4
  // cRZCallbackTimer
  HOOK_FUNC(_ZN16cRZCallbackTimerC1Ev)
  HOOK_FUNC(_ZN16cRZCallbackTimer13SetResolutionEj)
  HOOK_FUNC(_ZN16cRZCallbackTimer9SetPeriodEj)
  HOOK_FUNC(_ZN16cRZCallbackTimer9GetPeriodERj)
  HOOK_FUNC(_ZN16cRZCallbackTimer13StartPeriodicEjP11cIGZUnknown)
  HOOK_FUNC(_ZN16cRZCallbackTimer12StartOneShotEjP11cIGZUnknown)
  HOOK_FUNC(_ZN16cRZCallbackTimer4StopEv)
  HOOK_FUNC(_ZN16cRZCallbackTimerD1Ev)
  HOOK_FUNC(_ZN16cRZCallbackTimer19SetCallbackFunctionEPFvjEj)
  HOOK_FUNC(_ZN16cRZCallbackTimer19SetCallbackFunctionEPFvPvES0_)
  HOOK_FUNC(_ZN16cRZCallbackTimer20EnableSharedTimerUseEb)
  HOOK_FUNC(_ZN16cRZCallbackTimer9IsRunningEv)

  HOOK_FUNC(_ZN10cRZVersion12GetUserNameAER9cRZStringPS0_)
  HOOK_FUNC(_ZN10cRZVersion13GetModuleNameER9cRZString)
  HOOK_FUNC(_ZN10cRZVersion16GetModuleVersionER9cRZString)
  HOOK_FUNC(_ZN10cRZVersion11GetHostNameER9cRZStringPS0_)
  HOOK_FUNC(_ZN10cRZVersion11GetCPUSpeedEv)
  HOOK_FUNC(_ZN10cRZVersion15GetSystemMemoryERjS0_)

  HOOK_FUNC(_Z14RZFileFindNextP14RZFileFindData)
  HOOK_FUNC(_ZN12cRZDirectoryC1ERK9cRZStringbbb)

  //HOOK_FUNC(__ZN22nsGZLua5CompatWithLua47OpenLibEP8cIGZLua5)
#endif
#ifdef TARGET_GAME_sims2
  __ZN11nGZGraphic410cCanvasW3210WindowProcEP6HWND__jjl =
      reinterpret_cast<int (*)(SDL_Window *,
                               uint32_t,
                               uint32_t,
                               int32_t)>(gMachO->GetSymbolAddr(
          "__ZN11nGZGraphic410cCanvasW3210WindowProcEP6HWND__jjl"));

  HOOK_FUNC(_Z10IsFullPathPKc)
  //HOOK_FUNC(_ZN9cRZStringC2ERKS_jj)
  HOOK_FUNC(GetTimebase)
  hook_function(gMachO->GetSymbolAddr
  ("__ZN10cRZVersion16GetModuleVersionER9cRZString"), CAST_CPP_FUNCPTR
  (_ZN10cRZVersion16GetModuleVersionER9cRZString));
  hook_function(gMachO->GetSymbolAddr
  ("__ZN10cRZVersion13GetModuleNameER9cRZString"),
                CAST_CPP_FUNCPTR(_ZN10cRZVersion13GetModuleNameER9cRZString));
  hook_function(gMachO->GetSymbolAddr
  ("__ZN10cRZVersion11GetHostNameER9cRZStringPS0_"), CAST_CPP_FUNCPTR
  (_ZN10cRZVersion11GetHostNameER9cRZStringPS0_));
  hook_function(gMachO->GetSymbolAddr
  ("__ZN10cRZVersion15GetSystemMemoryERmS0_"), CAST_CPP_FUNCPTR
  (_ZN10cRZVersion15GetSystemMemoryERjS0_));

  hook_function(gMachO->GetSymbolAddr
  ("__ZN11nGZGraphic410cCanvasW325ClearEv"), CAST_CPP_FUNCPTR
  (__ZN11nGZGraphic410cCanvasW325ClearEv));

  hook_function(gMachO->GetSymbolAddr
  ("__ZN7cRZTime24GetMillisecondSystemTimeEv"), CAST_CPP_FUNCPTR
  (__ZN7cRZTime24GetMillisecondSystemTimeEv));

  hook_function(gMachO->GetSymbolAddr("__ZN15cRZThreadSignal9TimedWaitEm"),
                CAST_CPP_FUNCPTR(__ZN15cRZThreadSignal9TimedWaitEm));

  /*hook_function(gMachO->GetSymbolAddr
  ("__Z21ConvertStringEncodingPKvmlR10cIGZString"), CAST_CPP_FUNCPTR
  (_Z21ConvertStringEncodingPKvmlR10cIGZString));*/
#endif
}
