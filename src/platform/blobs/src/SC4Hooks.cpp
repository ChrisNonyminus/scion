
#include "SC4Hooks.h"
#ifdef TARGET_GAME_sc4

#include "cGDriver.h"

#include "Statics.h"

#include "cSC4RenderProperties.h"
#include "cRZString.h"

#define _GZCOM_CreatecSGLDriverDX7_ADDR 0x0042362C
#define __ZN16cGZGraphicSystem16ActiveDrawBufferEv_ADDR 0x00431D78
DECL_EXTERN_C_FUNC(_ZNK20cSC4RenderProperties9BoolValueEj,0x001E3D54)
#define __ZN7cSC4App16GetDataDirectoryER10cIGZStringl_ADDR 0x000062CA
DECL_EXTERN_C_FUNC(_GLOBAL__I_sVARegionMap,0x00AB6EEE)

void* _ZN16cGZGraphicSystem16ActiveDrawBufferEv(void* _this) {
  // stub
  return NULL;
}

void _GLOBAL__I_sVARegionMap() {
  ; // stub this function out
}

// tmp stub (TODO)
extern "C" void _ZN7cSC4App16GetDataDirectoryER10cIGZStringl(void* _this,
                                                        cIGZString& target,
                                                        long a3) {
  //printf("Data Directory before: %s\n", target.Data());
  //SC4GetUserDataDirectory(target, true);
  target.FromChar("Game/");
  ((cRZString*)((uint8_t*)_this + 0x80))->FromChar("Game/");
  //printf("Data Directory now: %s\n", target.Data());
}

void HookSC4Functions() {
  HOOK_FUNC(GZCOM_CreatecSGLDriverDX7);
  //HOOK_FUNC(_ZN16cGZGraphicSystem16ActiveDrawBufferEv)
  HOOK_FUNC(_ZNK20cSC4RenderProperties9BoolValueEj)
  HOOK_FUNC(_ZN7cSC4App16GetDataDirectoryER10cIGZStringl)

  HOOK_FUNC(_GLOBAL__I_sVARegionMap)
}
#else
void HookSC4Functions() {}
#endif