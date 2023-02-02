//
// Created by chris on 1/17/2023.
//

#ifndef SCION_SC4WIN_H
#define SCION_SC4WIN_H

#include "cIGZUnknown.h"

inline uint32_t GZCOM_cSC4WinAdviceListCLSID() { return 0xCA1492AC; }
cIGZUnknown* GZCOM_CreatecSC4WinAdviceList();
inline uint32_t GZCOM_cSC4WinMiniMapCLSID() { return 0xCA318388; }
cIGZUnknown* GZCOM_CreatecSC4WinMiniMap();
inline uint32_t GZCOM_cSC4WinAuraBarCLSID() { return 0xAA5D16A9; }
cIGZUnknown* GZCOM_CreatecSC4WinAuraBar();
inline uint32_t GZCOM_cSC4WinAlertBorderCLSID() { return 0xCA5D3294; }
cIGZUnknown* GZCOM_CreatecSC4WinAlertBorder();
inline uint32_t GZCOM_cSC4WinTextCLSID() { return 0xAA7CECFD; }
cIGZUnknown* GZCOM_CreatecSC4WinText();

#endif //SCION_SC4WIN_H
