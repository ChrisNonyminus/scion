#ifndef BLOBS_SRC_MAC_HOOKS_H
#define BLOBS_SRC_MAC_HOOKS_H

#include <SDL2/SDL.h>

#include "hooks.h"

#include <string>
#include <vector>

// CoreGraphics
struct io_object_t {
  std::vector<std::string> validClassNames;
};
struct _CGDirectDisplayID {
  int displayIdx;
};
extern "C" {
};

void HookMiscCarbonFunctions();
void HookCoreFoundationFunctions();
void HookCoreGraphicsFunctions();

#endif //BLOBS_SRC_MAC_HOOKS_H
