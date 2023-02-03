#include "Registry.h"

#include <iostream>
#include <fstream>

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include <map>

namespace Win32API {
struct RegistryKey {
  std::string name;
  std::string value;
  int type;
  std::vector<RegistryKey> subkeys;

  std::vector<RegistryKey>::iterator begin() {
    return subkeys.begin();
  }

  std::vector<RegistryKey>::iterator end() {
    return subkeys.end();
  }
};

struct RegistryKeyList {
  std::vector<RegistryKey> keys;
  RegistryKey defaultKey;

  // begin and end are used for iterating over the keys in a RegistryKeyList
  std::vector<RegistryKey>::iterator begin() {
    return keys.begin();
  }

  std::vector<RegistryKey>::iterator end() {
    return keys.end();
  }
};

RegistryKeyList &GetRegistryKeyList(const char *key);
std::string GetRegistryValue(const char *key, const char *value);
void SetRegistryKey(const char *key, const char *value, const char *data);

void InitRegistry();

std::map<std::string, RegistryKeyList> gRegistry;
}

Win32API::RegistryKeyList &Win32API::GetRegistryKeyList(const char *key) {
  return gRegistry[key];
}

std::string Win32API::GetRegistryValue(const char *key, const char *value) {
  RegistryKeyList &keyList = GetRegistryKeyList(key);
  /*for (auto &k : keyList.keys) {
    if (k.name == value) {
      return k.value;
    }
  }*/
  for (int i = 0; i < keyList.keys.size(); i++) {
    if (keyList.keys[i].name == key)
      return keyList.keys[i].value;
  }
  printf("[ERROR] GetRegistryValue: Key %s not found.\n", key);
  signal(SIGSEGV, SIG_DFL);
  return "";
}

void Win32API::SetRegistryKey(const char *key,
                              const char *value,
                              const char *data) {
/*  RegistryKeyList &keyList = GetRegistryKeyList(key);
  for (auto &k : keyList.keys) {
    if (k.name == value) {
      k.value = data;
      return;
    }
  }
  RegistryKey newKey;
  newKey.name = value;
  newKey.value = data;
  keyList.keys.push_back(newKey);*/
}

#ifdef TARGET_GAME_sims2
enum Sims2Apps {
  SIMS2_APP = 0,
  SIMS2_EP1_APP,
  SIMS2_EP2_APP,
  SIMS2_EP3_APP,
  SIMS2_EP4_APP,
  SIMS2_EP5_APP,
  SIMS2_EP6_APP,
  //SIMS2_EP7_APP,
};
#endif

// Initialize the registry keys the game uses. Don't use a registry file.
void Win32API::InitRegistry() {

#ifdef TARGET_GAME_sims2
  static const char *mAppPathsKey =
      "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\";
  static const char *mAppNames[] = {
      "Sims2.exe",
      "Sims2EP1.exe",
      "Sims2EP2.exe",
      "Sims2EP3.exe",
      "Sims2EP4.exe",
      "Sims2EP5.exe",
      "Sims2EP6.exe",
      //"Sims2EP7.exe",
      // "Sims2EP8.exe",
      // "Sims2EP9.exe",
  };
  static const char *mAppPaths[] = {
      "Double Deluxe\\Base\\",
      "University Life\\EP1\\",
      "Double Deluxe\\EP2\\",
      "Best of Business\\EP3\\",
      "Fun with Pets\\EP4\\",
      "Seasons\\",
      "Bon Voyage\\",
      // "Sims2EP7",
      // "Sims2EP8",
      // "Sims2EP9",
  };
  for (int i = 0; i < sizeof(mAppNames) / sizeof(mAppNames[0]); i++) {
    std::string name = mAppNames[i];
    std::string key = mAppPathsKey;
    key += name;
    if (gRegistry.find(key) == gRegistry.end()) {
      gRegistry[key] = RegistryKeyList();
    }
    RegistryKeyList &keyList = GetRegistryKeyList(key.c_str());
    RegistryKey newKey;

    newKey.name = "Game Registry"; // This points to the game's registry key.
    std::string value = "Software\\EA GAMES\\";
    value += name;
    newKey.value = value;
    newKey.type = 1;
    keyList.keys.push_back(newKey);

    newKey.name = "Path";
    std::string path = getenv("HOME");
    path += "/.scion/Sims2/Game/";
    path += mAppPaths[i];
    newKey.value = path;
    newKey.type = 1;
    keyList.keys.push_back(newKey);

    newKey.name = "Installed";
    newKey.value = "1";
    newKey.type = 4;
    keyList.keys.push_back(newKey);

    newKey.name = "Restart";
    newKey.value = "0";
    newKey.type = 4;
    keyList.keys.push_back(newKey);

    keyList.defaultKey = newKey;
    keyList.defaultKey.name = "";
    keyList.defaultKey.value = path + "TSBin\\" + mAppNames[i];
    keyList.defaultKey.type = 1;

  }
  // initialize the main game registry key ("Software\\EA GAMES\\Sims2.exe")
  std::string key = "Software\\EA GAMES\\Sims2.exe";
  if (gRegistry.find(key) == gRegistry.end()) {
    gRegistry[key] = RegistryKeyList();
  }
  RegistryKeyList &keyList = GetRegistryKeyList(key.c_str());
  RegistryKey newKey;

  newKey.name = "CacheSize";
  newKey.value = "4735180800";
  newKey.type = 4;
  keyList.keys.push_back(newKey);

  newKey.name = "CD Drive";
  newKey.value = "D:\\";
  newKey.type = 1;
  keyList.keys.push_back(newKey);

  newKey.name = "DisplayName";
  newKey.value = "The Sims 2";
  newKey.type = 1;
  keyList.keys.push_back(newKey);

  newKey.name =
      "EPsInstalled"; // String value of each EP exe name separated by a comma.
  newKey.value =
      "Sims2EP1.exe,Sims2EP2.exe,Sims2EP3.exe,Sims2EP4.exe,Sims2EP5.exe,"
      "Sims2EP6.exe";
  newKey.type = 1;
  keyList.keys.push_back(newKey);

  newKey.name = "Folder"; // ~/.scion/Sims2/Game/
  newKey.value = getenv("HOME");
  newKey.value += "/.scion/Sims2/Game/";
  newKey.type = 1;
  keyList.keys.push_back(newKey);

  newKey.name = "Install Dir";
  newKey.value += "Double Deluxe\\Base\\";
  newKey.type = 1;
  keyList.keys.push_back(newKey);



#endif

  key = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
  if (gRegistry.find(key) == gRegistry.end()) {
    gRegistry[key] = RegistryKeyList();
  }
  RegistryKeyList &keyList2 = GetRegistryKeyList(key.c_str());

  RegistryKey cpuKey;
  cpuKey.type = 4;
  cpuKey.name = "~MHz";
  cpuKey.value = "2000";
  keyList2.keys.push_back(cpuKey);


}

int RegOpenKeyExA(void *hKey,
                  const char *lpSubKey,
                  unsigned long ulOptions,
                  unsigned long samDesired,
                  void **phkResult) {
  std::string key = lpSubKey;
  if (Win32API::gRegistry.find(key) == Win32API::gRegistry.end()) {
    printf("[ERROR] RegOpenKeyExA: Key %s not found.\n", key.c_str());
    return 2;
  }
  const char *keyName = key.c_str();
  Win32API::RegistryKeyList &keyList = Win32API::GetRegistryKeyList(keyName);
  *phkResult = (void *) &keyList;
  return 0;
}
int RegCloseKey(void *hKey) {
  return 0;
}
int RegQueryValueExA(void *hKey,
                     const char *lpValueName,
                     unsigned long *lpReserved,
                     unsigned long *lpType,
                     unsigned char *lpData,
                     unsigned long *lpcbData) {
  if (lpValueName == NULL) {
    // Return the type and data for the key's unnamed or default value, if any.
    Win32API::RegistryKeyList &keyList = *(Win32API::RegistryKeyList *) hKey;
    *lpType = keyList.defaultKey.type;
    switch (keyList.defaultKey.type) {
    case 1: // REG_SZ
      strcpy((char *) lpData, keyList.defaultKey.value.c_str());
      *lpcbData = keyList.defaultKey.value.size();
      return 0;
    case 4: // REG_DWORD
      *(unsigned long *) lpData = atoi(keyList.defaultKey.value.c_str());
      *lpcbData = 4;
      return 0;
    default:
      printf("[ERROR] RegQueryValueExA: Unsupported type %d.\n",
             *lpType);
      return 2;
    }

  }
  Win32API::RegistryKeyList &keyList = *(Win32API::RegistryKeyList *) hKey;
  for (int i = 0; i < keyList.keys.size(); i++) {
    Win32API::RegistryKey &key = keyList.keys[i];
    if (key.name == lpValueName) {
      switch (key.type) {
      case 1: // REG_SZ
        strcpy((char *) lpData, key.value.c_str());
        *lpcbData = key.value.size();
        return 0;
      case 4: // REG_DWORD
        *(unsigned long *) lpData = atoi(key.value.c_str());
        *lpcbData = 4;
        return 0;
      default:
        printf("[ERROR] RegQueryValueExA: Unsupported type %d.\n",
               *lpType);
        return 2;
      }
    }
  }
  printf("[ERROR] RegQueryValueExA: Key %s not found.\n", lpValueName);
  return 2;
}
int RegSetValueExA(void *hKey,
                   const char *lpValueName,
                   unsigned long Reserved,
                   unsigned long dwType,
                   const unsigned char *lpData,
                   unsigned long cbData) {
  Win32API::RegistryKeyList &keyList = *(Win32API::RegistryKeyList *) hKey;
  for (int i = 0; i < keyList.keys.size(); i++) {
    Win32API::RegistryKey &key = keyList.keys[i];
    if (key.name == lpValueName) {
      switch (dwType) {
      case 1: // REG_SZ
        key.value = (char *) lpData;
        return 0;
      case 4: // REG_DWORD
        key.value = /*std::to_string(*(unsigned long *) lpData)*/(char *)
            lpData; // TODO
        return 0;
      default:printf("[ERROR] RegSetValueExA: Unsupported type %d.\n", dwType);
        return 2;
      }
    }
  }
  printf("[ERROR] RegSetValueExA: Key %s not found.\n", lpValueName);
  return 2;
}
