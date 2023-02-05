
#include "RZWinAPI.h"

#include <locale>
#include <dlfcn.h>

#ifdef TARGET_GAME_sims2
#define _WideStringFromCString_ADDR 0xe09a
DECL_EXTERN_C_FUNC(_Z25ProcessSignatureIsRunningj, 0x005B36BA)
#include "../The Sims 2 Bon Voyage~.x86_winapi_addrs.txt"
#define _ShowWindow_Win32_ADDR 0x00040262
#endif

#include "Win32API/Registry.h"
#include "Win32API/Threading.h"
#include "Win32API/FS.h"
#include "Win32API/DirectX/DirectSound.h"
#include "Win32API/Locale.h"
#include "Win32API/GDI.h"
#include "Win32API/Windowing.h"
#include "Win32API/Misc.h"
#include "Win32API/DirectX/Direct3D.h"
#include "Win32API/Input.h"

#include "ld-mac.hh"

cRZFileAPIW32::cRZFileAPIW32() {

}

cRZDirAPIW32::cRZDirAPIW32() {

}

bool ProcessSignatureIsRunning(uint32_t sig) {
  return false; // stub (TODO)
}

void _ZN36ShellExecuteFunctionEntryPointFinderC2Ev() {}

uint32_t WideStringFromCString(wchar_t *dst, char *src, int len, int, int) {
  std::mbstowcs(dst, src, len);
  dst[len] = 0;
  return 0;
}

void OutputDebugStringA(const char *lpOutputString) {
  printf("[OutputDebugStringA] %s\n", lpOutputString);
}

int LoadLibraryA(const char *szLibName) {
  return 0; // TODO: tmp stub
}

void DoRZWinAPIHooks() {
  Win32API::InitRegistry();

  HOOK_FUNC(_ZN12cRZDirAPIW32C2Ev)
  HOOK_FUNC(_ZN13cRZFileAPIW32C2Ev)
  HOOK_FUNC(_ZN36ShellExecuteFunctionEntryPointFinderC2Ev)
  HOOK_FUNC(WideStringFromCString)
  HOOK_FUNC(_Z25ProcessSignatureIsRunningj)
  HOOK_FUNC(LoadLibraryA)

  HOOK_FUNC(GetModuleHandleA)

  HOOK_FUNC(RegOpenKeyExA)
  HOOK_FUNC(RegQueryValueExA)
  HOOK_FUNC(RegCloseKey)
  HOOK_FUNC(RegSetValueExA)

  HOOK_FUNC(GetFileAttributesA)
  HOOK_FUNC(CreateFileA)
  HOOK_FUNC(ReadFile)
  HOOK_FUNC(WriteFile)
  HOOK_FUNC(CreateDirectoryW)
  HOOK_FUNC(RemoveDirectoryW)
  HOOK_FUNC(DeleteFileA)
  HOOK_FUNC(FindFirstFileA)
  HOOK_FUNC(FindFirstFileW)
  HOOK_FUNC(FindNextFileA)
  HOOK_FUNC(FindNextFileW)
  HOOK_FUNC(FindClose)
  HOOK_FUNC(SHGetFolderPathA)
  HOOK_FUNC(SHGetFolderPathW)
  HOOK_FUNC(GetFileSize)
  HOOK_FUNC(SetEndOfFile)
  HOOK_FUNC(SetFilePointer)
  HOOK_FUNC(CopyFileA)
  HOOK_FUNC(FlushFileBuffers)

  HOOK_FUNC(InterlockedExchangeAdd)
  HOOK_FUNC(GetCurrentThreadId)
  HOOK_FUNC(SetThreadExecutionState)
  HOOK_FUNC(InterlockedIncrement)
  HOOK_FUNC(InterlockedDecrement)

  HOOK_FUNC(OutputDebugStringA)

  HOOK_FUNC(CloseHandle)

  HOOK_FUNC(DirectSoundCreate)
  HOOK_FUNC(Direct3DCreate9)
  //HOOK_FUNC(D3DXLoadSurfaceFromMemory)

  HOOK_FUNC(GetLocaleInfoA)

  HOOK_FUNC(CreateSolidBrush)
  HOOK_FUNC(CreateWindowExA)
  HOOK_FUNC(ShowWindow_Win32)
  HOOK_FUNC(UpdateWindow)
  HOOK_FUNC(GetClientRect)
  HOOK_FUNC(ClientToScreen)
  HOOK_FUNC(GetMonitorInfoA)
  HOOK_FUNC(CreateDCA)
  HOOK_FUNC(GetDeviceCaps)
  HOOK_FUNC(DeleteDC)
  HOOK_FUNC(MonitorFromWindow)
  HOOK_FUNC(LoadCursorA)
  HOOK_FUNC(PeekMessageA)
  HOOK_FUNC(InvalidateRect)
  HOOK_FUNC(SetActiveWindow)
  HOOK_FUNC(PostMessageA)
  HOOK_FUNC(LoadCursorFromFileA)
  HOOK_FUNC(DestroyCursor)
  HOOK_FUNC(DispatchMessageA)
  HOOK_FUNC(GetWindowLongA)
  HOOK_FUNC(SetWindowLongA)
  HOOK_FUNC(GetActiveWindow)
  HOOK_FUNC(SetWindowTextA)
  HOOK_FUNC(BeginPaint)
  HOOK_FUNC(EndPaint)
  HOOK_FUNC(SendMessageA)

  HOOK_FUNC(GetTickCount)
  HOOK_FUNC(timeGetTime)
  HOOK_FUNC(GetUserNameA);

  HOOK_FUNC(GetKeyState)
#ifdef TARGET_GAME_sims2
//#include "../The Sims 2 Bon Voyage~.x86_winapi_hooks.txt"
#endif
  hook_function(gMachO->GetSymbolAddr("__Z9SplitpathPKcPcS1_S1_S1_"),
                CAST_CPP_FUNCPTR(__Z9SplitpathPKcPcS1_S1_S1_));
}
