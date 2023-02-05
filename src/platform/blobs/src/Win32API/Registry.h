/*
    Reimplementation of the Win32 API's Registry functions, for use on Linux/SDL setups.
*/

#pragma once

#include <string>
#include <vector>

extern "C" {
int RegOpenKeyExA(void *hKey,
                  const char *lpSubKey,
                  unsigned long ulOptions,
                  unsigned long samDesired,
                  void **phkResult);
int RegCloseKey(void *hKey);
int RegQueryValueExA(void *hKey,
                     const char *lpValueName,
                     unsigned long *lpReserved,
                     unsigned long *lpType,
                     unsigned char *lpData,
                     unsigned long *lpcbData);
int RegSetValueExA(void *hKey,
                   const char *lpValueName,
                   unsigned long Reserved,
                   unsigned long dwType,
                   const unsigned char *lpData,
                   unsigned long cbData);
}

namespace Win32API {
void InitRegistry();
}

std::string GetSims2BaseGameTSDataDir();
std::string ResolveSims2BaseGameTSDataPath(std::string path);