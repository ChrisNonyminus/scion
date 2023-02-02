
#include <glad/gl.h>
#include <GL/gl.h>

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <unistd.h>
#endif

#include "MacTools.h"

#include <SDL2/SDL.h>

#ifdef TARGET_GAME_sc4
#define _MacTools_FindCDROM_ADDR 0x00450C94
#define _InterlockedIncrement_ADDR 0x004513A4
#define _InterlockedDecrement_ADDR 0x004513C0
#define _MacTools_MessageBox_ADDR 0x004512FE
#define _MacDisplay_GetCurrentMode_ADDR 0x0044F504
#define _MacDisplay_GetVideoMemoryInfo_ADDR 0x0044F5DC
#define _MacTools_Sleep_ADDR 0x00450E6C
#endif

#include "cGZGraphicSystem.h"

void MacDisplay::GetCurrentMode(int &width, int &height, int &depth) {
#ifdef TARGET_GAME_sc4
  // TODO: remove this
  // hack: modify cGZGraphicSystem::mDesiredStartupDisplayMetrics
  // (which should be private but in this case we have to make it not
  // so) to reflect the best display mode.
  if (cGZGraphicSystem::mDesiredStartupDisplayMetrics.height < 480) {
    cGZGraphicSystem::mDesiredStartupDisplayMetrics.width = 1280;
    cGZGraphicSystem::mDesiredStartupDisplayMetrics.height = 720;
    cGZGraphicSystem::mDesiredStartupDisplayMetrics.bpp = 32;
  }
#endif
  // TODO
  width = 1280;
  height = 720;
  depth = 32;
}
void MacDisplay::GetVideoMemoryInfo(long &videoMemory, long &textureMemory) {
  videoMemory = 0;
  textureMemory = 0;

#ifdef TARGET_GAME_sc4
  int vram, tmem;
  glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vram);
  glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &tmem);
  videoMemory = vram;
  textureMemory = tmem;
#endif
}

namespace MacTools {
int FindCDROM(const char *) { return 1; } // We don't need the CDROM
void MessageBox(const char *msg, uint8_t) {
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                           "Scion",
                           msg,
                           SDL_GL_GetCurrentWindow());
}
void Sleep(uint32_t duration) {
  SDL_Delay(duration);
  //return 0;
}
}


void HookMacTools() {
#ifdef TARGET_GAME_sc4
  hook_function(_MacTools_FindCDROM_ADDR,
                CAST_CPP_FUNCPTR(MacTools::FindCDROM));
  hook_function(_MacTools_MessageBox_ADDR,
                CAST_CPP_FUNCPTR(MacTools::MessageBox));
  hook_function(_MacDisplay_GetCurrentMode_ADDR,
                CAST_CPP_FUNCPTR(MacDisplay::GetCurrentMode));
  hook_function(_MacDisplay_GetVideoMemoryInfo_ADDR,
                CAST_CPP_FUNCPTR(MacDisplay::GetVideoMemoryInfo));
  hook_function(_MacTools_Sleep_ADDR,
                CAST_CPP_FUNCPTR(MacTools::Sleep));
#endif
}