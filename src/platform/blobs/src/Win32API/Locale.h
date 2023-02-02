#ifndef BLOBS_SRC_WIN32API_LOCALE_H
#define BLOBS_SRC_WIN32API_LOCALE_H

#include "WinUtils.h"

extern "C" uint32_t GetLocaleInfoA(
    /*in*/ int Locale,
    /*in*/ int LCType,
    /*out, optional*/ char *lpLCData,
    /*in*/ int cchData
);

#endif //BLOBS_SRC_WIN32API_LOCALE_H
