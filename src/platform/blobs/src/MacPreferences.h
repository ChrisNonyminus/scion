#ifndef BLOBS_MACPREFERENCES_H
#define BLOBS_MACPREFERENCES_H

#include "GZTypes.h"

#include "hooks.h"


struct __CFString {
  void *isa;
  int unk;
  char *str;
  int unk2;
};

typedef __CFString* CFStringRef;

struct CGPoint {
  double x, y;
};

struct CGRect {
  CGPoint origin, size;
};


namespace MacPreferences {
void GetCFString(const char *key, const __CFString **a2, const char *a3);
void PutCFString(const char *key, const __CFString *value);

void GetString(const char *key, char *a2, int a3, const char *a4);
void PutString(const char *key, const char *value);

int GetInteger(const char *key, int def);
void PutInteger(const char *key, int value);

float GetFloat(const char *key, float def);
void PutFloat(const char *key, float value);

bool GetBoolean(const char *key, bool def);
void PutBoolean(const char *key, bool value);

void GetRect(const char *key, CGRect &value);
void PutRect(const char *key, const CGRect &value);

void Synchronize();

}

namespace ASL {
bool GetPrefAsBoolean(CFStringRef key, bool defaultVal);
short GetPrefAsSInt16(CFStringRef key, short defaultVal);
int32_t GetPrefAsSInt32(CFStringRef key, int32_t defaultVal);
uint32_t GetPrefAsUInt32(CFStringRef key, uint32_t defaultVal);
const void *GetPrefAsDate(CFStringRef key);

void SetPrefAsBoolean(CFStringRef key, bool value);
void SetPrefAsSInt16(CFStringRef key, short value);
void SetPrefAsSInt32(CFStringRef key, int32_t value);
void SetPrefAsUInt32(CFStringRef key, uint32_t value);
void SetPrefAsDate(CFStringRef key, const void *value);

void FlushPreferencesToDisk();
}

void DoASLHooks();

#endif //BLOBS_MACPREFERENCES_H
