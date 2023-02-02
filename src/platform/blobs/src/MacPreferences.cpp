
#include "MacPreferences.h"

// Linux implementation of MacPreferences

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include <boost/filesystem.hpp>

namespace MacPreferences {

static void GetPrefsPath(char *path) {
  const char *home = getenv("HOME");
  strcpy(path, home);
  strcat(path, "/.scion/Global");

  strcat(path, "/MacShimPrefs.txt");

  // create directory if it doesn't exist
  mkdirpwrapper(path);

  // create file if it doesn't exist
  FILE *tmp = fopen(path, "r");
  if (!tmp) {
    tmp = fopen(path, "w");
  }
  fclose(tmp);

}

void GetCFString(const char *key, const __CFString **a2, const char *a3) {
  char path[1024];
  GetPrefsPath(path);

  FILE *f = fopen(path, "r");
  if (f) {
    char line[1024];
    while (fgets(line, 1024, f)) {
      if (strncmp(line, key, strlen(key)) == 0) {
        char *value = strchr(line, '=');
        if (value) {
          value++;
          int len = strlen(value);
          if (value[len - 1] == '\n') {
            value[len - 1] = 0;
          }
          __CFString *str = (__CFString *) malloc(sizeof(__CFString));
          str->str = strdup(value);
          *a2 = str;
          return;
        }
      }
    }
  }

  __CFString *str = (__CFString *) malloc(sizeof(__CFString));
  str->str = strdup(a3);
  *a2 = str;
  PutCFString(key, *a2);
}

void PutCFString(const char *key, const __CFString *value) {
  char path[1024];
  GetPrefsPath(path);

  // add key=value to file
  // replace instance of key=<???> if it exists
  // otherwise append to end of file

  FILE *f = fopen(path, "r");
  if (f) {
    char tmp[260];
    strcpy(tmp, path);
    strcat(tmp, ".tmp");
    FILE *f2 = fopen(tmp, "w");
    if (f2) {
      char line[1024];
      bool found = false;
      while (fgets(line, 1024, f)) {
        if (strncmp(line, key, strlen(key)) == 0) {
          fprintf(f2, "%s=%s\n", key, value->str);
          found = true;
        } else {
          fprintf(f2, "%s", line);
        }
      }
      if (!found) {
        fprintf(f2, "%s=%s\n", key, value->str);
      }
      fclose(f2);
    }
    fclose(f);
    boost::filesystem::rename(tmp, path);
  } else {
    f = fopen(path, "w");
    if (f) {
      fprintf(f, "%s=%s\n", key, value->str);
      fclose(f);
    }
  }
}

void GetString(const char *key, char *a2, int a3, const char *a4) {
  const __CFString *str = NULL;
  GetCFString(key, &str, a4);
  if (str) {
    strcpy(a2, str->str);
    free((void *) str->str);
    free((void *) str);
  }
}

void PutString(const char *key, const char *value) {
  __CFString *str = (__CFString *) malloc(sizeof(__CFString));
  str->str = strdup(value);
  PutCFString(key, str);
  free((void *) str->str);
  free((void *) str);
}

int GetInteger(const char *key, int def) {
  const __CFString *str = NULL;
  char buf[32];
  sprintf(buf, "%d", def);
  GetCFString(key, &str, buf);
  if (str) {
    int value = atoi(str->str);
    free((void *) str->str);
    free((void *) str);
    return value;
  }
  return def;
}

void PutInteger(const char *key, int value) {
  char buf[32];
  sprintf(buf, "%d", value);
  PutString(key, buf);
}

bool GetBoolean(const char *key, bool def) {
  const __CFString *str = NULL;
  char buf[32];
  sprintf(buf, "%d", def);
  GetCFString(key, &str, buf);
  if (str) {
    bool value = atoi(str->str) != 0;
    free((void *) str->str);
    free((void *) str);
    return value;
  }
  return def;
}

void PutBoolean(const char *key, bool value) {
  char buf[32];
  sprintf(buf, "%d", value);
  PutString(key, buf);
}

float GetFloat(const char *key, float def) {
  const __CFString *str = NULL;
  char buf[32];
  sprintf(buf, "%f", def);
  GetCFString(key, &str, buf);
  if (str) {
    float value = atof(str->str);
    free((void *) str->str);
    free((void *) str);
    return value;
  }
  return def;
}

void PutFloat(const char *key, float value) {
  char buf[32];
  sprintf(buf, "%f", value);
  PutString(key, buf);
}

void GetRect(const char *key, CGRect &value) {
  char buf[256];
  sprintf(buf,
          "%f,%f,%f,%f",
          value.origin.x,
          value.origin.y,
          value.size.x,
          value.size.y);
  GetString(key, buf, 256, buf);
  sscanf(buf,
         "%f,%f,%f,%f",
         &value.origin.x,
         &value.origin.y,
         &value.size.x,
         &value.size.y);
}

void PutRect(const char *key, const CGRect &value) {
  char buf[256];
  sprintf(buf,
          "%f,%f,%f,%f",
          value.origin.x,
          value.origin.y,
          value.size.x,
          value.size.y);
  PutString(key, buf);
}

void Synchronize() {

}

}
bool ASL::GetPrefAsBoolean(CFStringRef key, bool defaultVal) {
  return MacPreferences::GetBoolean(key->str, defaultVal);

}
short ASL::GetPrefAsSInt16(CFStringRef key, short defaultVal) {
  return MacPreferences::GetInteger(key->str, defaultVal);
}
int32_t ASL::GetPrefAsSInt32(CFStringRef key, int32_t defaultVal) {
  return MacPreferences::GetInteger(key->str, defaultVal);
}
uint32_t ASL::GetPrefAsUInt32(CFStringRef key, uint32_t defaultVal) {
  const __CFString *str = NULL;
  char buf[32];
  sprintf(buf, "0x%X", defaultVal);
  MacPreferences::GetCFString(key->str, &str, buf);
  if (str) {
    uint32_t value = strtoul(str->str, NULL, 16);
    free((void *) str->str);
    free((void *) str);
    return value;
  }
  return defaultVal;
}
const void *ASL::GetPrefAsDate(CFStringRef key) {
  return NULL;
}
void ASL::SetPrefAsBoolean(CFStringRef key, bool value) {
  MacPreferences::PutBoolean(key->str, value);
}
void ASL::SetPrefAsSInt16(CFStringRef key, short value) {
  MacPreferences::PutInteger(key->str, value);
}
void ASL::SetPrefAsSInt32(CFStringRef key, int32_t value) {
  MacPreferences::PutInteger(key->str, value);
}
void ASL::SetPrefAsUInt32(CFStringRef key, uint32_t value) {
  char buf[32];
  sprintf(buf, "0x%X", value);
  MacPreferences::PutString(key->str, buf);
}
void ASL::SetPrefAsDate(CFStringRef key, const void *value) {

}
void ASL::FlushPreferencesToDisk() {
  MacPreferences::Synchronize();
}

#ifdef TARGET_GAME_sims2
#define __ZN3ASL16GetPrefAsBooleanEPK10__CFStringh_ADDR 0x0000401C
#define __ZN3ASL15GetPrefAsSInt16EPK10__CFStrings_ADDR 0x4058
#define __ZN3ASL15GetPrefAsSInt32EPK10__CFStringl_ADDR 0x408E
#define __ZN3ASL15GetPrefAsUInt32EPK10__CFStringm_ADDR 0x40C4

#define __ZN3ASL16SetPrefAsBooleanEPK10__CFStringh_ADDR 0x414e
#define __ZN3ASL15SetPrefAsSInt16EPK10__CFStrings_ADDR 0x4190
#define __ZN3ASL15SetPrefAsSInt32EPK10__CFStringl_ADDR 0x41e8
#define __ZN3ASL15SetPrefAsUInt32EPK10__CFStringm_ADDR 0x4238
#define __ZN3ASL22FlushPreferencesToDiskEv_ADDR 0x42ae

#endif

void DoASLHooks() {
#ifdef TARGET_GAME_sims2
  hook_function(__ZN3ASL16GetPrefAsBooleanEPK10__CFStringh_ADDR,
                (void *) ASL::GetPrefAsBoolean);
  hook_function(__ZN3ASL15GetPrefAsSInt16EPK10__CFStrings_ADDR,
                (void *) ASL::GetPrefAsSInt16);
  hook_function(__ZN3ASL15GetPrefAsSInt32EPK10__CFStringl_ADDR,
                (void *) ASL::GetPrefAsSInt32);
  hook_function(__ZN3ASL15GetPrefAsUInt32EPK10__CFStringm_ADDR,
                (void *) ASL::GetPrefAsUInt32);

  hook_function(__ZN3ASL16SetPrefAsBooleanEPK10__CFStringh_ADDR,
                (void *) ASL::SetPrefAsBoolean);
  hook_function(__ZN3ASL15SetPrefAsSInt16EPK10__CFStrings_ADDR,
                (void *) ASL::SetPrefAsSInt16);
  hook_function(__ZN3ASL15SetPrefAsSInt32EPK10__CFStringl_ADDR,
                (void *) ASL::SetPrefAsSInt32);
  hook_function(__ZN3ASL15SetPrefAsUInt32EPK10__CFStringm_ADDR,
                (void *) ASL::SetPrefAsUInt32);
  hook_function(__ZN3ASL22FlushPreferencesToDiskEv_ADDR,
                (void *) ASL::FlushPreferencesToDisk);
#endif
}