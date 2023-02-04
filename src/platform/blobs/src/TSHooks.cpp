#include "TSHooks.h"
#include "cRZString.h"

#include "ld-mac.hh"

#ifdef TARGET_GAME_sims2

/*
 * all the stubs/temporary pre-class-implementation hooks
 * */

extern "C" void
__ZN23cTSConfigurationManager15ParseConfigFileEP20cTSConfigParserState() {
  ; // STUB
}

extern "C" void __ZN16cTSWinCalloutBox12RecalcLayoutEv() {
  ; // the actual code seems to crash right now
}

extern "C" void _ZN16cTSPersistSystem25OpenResourcesInBackgroundEv();

void HookSims2Functions() {
  /*hook_function(gMachO->GetSymbolAddr
  ("__ZN23cTSConfigurationManager15ParseConfigFileEP20cTSConfigParserState"),
                CAST_CPP_FUNCPTR
                (__ZN23cTSConfigurationManager15ParseConfigFileEP20cTSConfigParserState));*/
  /*hook_function(gMachO->GetSymbolAddr("__ZN16cTSWinCalloutBox12RecalcLayoutEv"),
                CAST_CPP_FUNCPTR(__ZN16cTSWinCalloutBox12RecalcLayoutEv));*/

  hook_function(gMachO->GetSymbolAddr
  ("__ZN16cTSPersistSystem25OpenResourcesInBackgroundEv"), CAST_CPP_FUNCPTR
  (_ZN16cTSPersistSystem25OpenResourcesInBackgroundEv));
}
bool HackyFunctionToCheckIfResourcesAreAllLoaded() {
  return false;
}

#else
void HookSims2Functions() {}
#endif