#include <cstring>
#include "Locale.h"
uint32_t GetLocaleInfoA(int Locale, int LCType, char *lpLCData, int cchData) {
  if (lpLCData != NULL)
    strcpy(lpLCData, "16");
  return 0;
}
