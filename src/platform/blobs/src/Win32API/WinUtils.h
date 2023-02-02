#ifndef WINUTILS_H
#define WINUTILS_H

#include <stdint.h>
#include <string>
#include <cstdio>

std::string guid_to_string(uint8_t *lpGUID);

struct IUnknown {
  virtual uint32_t QueryInterface(uint8_t *guid, void **ppvObject) = 0;
  virtual uint32_t AddRef() = 0;
  virtual uint32_t Release() = 0;
};

#define DWORD uint32_t
#define LONG int32_t

extern uint32_t sLastError;

#endif
