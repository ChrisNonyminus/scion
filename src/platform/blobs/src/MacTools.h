#ifndef BLOBS_SRC_MACTOOLS_H
#define BLOBS_SRC_MACTOOLS_H

#include <SDL2/SDL.h>

#include "hooks.h"

namespace MacTools {
int FindCDROM(const char *);
void MessageBox(const char *msg, uint8_t);
void Sleep(uint32_t duration);
}
namespace MacDisplay {
void GetCurrentMode(int &width, int &height, int &depth);
void GetVideoMemoryInfo(long &videoMemory, long &textureMemory);
}


void HookMacTools();
#endif //BLOBS_SRC_MACTOOLS_H
