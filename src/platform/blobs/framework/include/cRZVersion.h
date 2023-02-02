#ifndef BLOBS_FRAMEWORK_INCLUDE_CRZVERSION_H
#define BLOBS_FRAMEWORK_INCLUDE_CRZVERSION_H

#include "cRZString.h"

class cRZVersion {
public:
  int GetUserNameA(cRZString& targetName, cRZString* a3);
  int GetModuleName(cRZString& target);
  int GetModuleVersion(cRZString& target);
  int GetHostName(cRZString& target, cRZString* a3);
  int GetCPUSpeed();
  int GetSystemMemory(
      uint32_t& memUsed/*possibly RAM, in B*/,
      uint32_t& memFree/*possibly VRAM, in B*/
      );
};

#endif //BLOBS_FRAMEWORK_INCLUDE_CRZVERSION_H
