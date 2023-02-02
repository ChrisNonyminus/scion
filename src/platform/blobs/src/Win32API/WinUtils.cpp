#include "WinUtils.h"
std::string guid_to_string(uint8_t *lpGUID) {
  if (!lpGUID)
    return "(null GUID)";

  char tmp[256];
  sprintf(tmp,
          "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
          *(uint32_t *) &lpGUID[0],
          *(uint16_t *) &lpGUID[4],
          *(uint16_t *) &lpGUID[6],
          lpGUID[8],
          lpGUID[9],
          lpGUID[10],
          lpGUID[11],
          lpGUID[12],
          lpGUID[13],
          lpGUID[14],
          lpGUID[15]);

  return std::string(tmp);
}