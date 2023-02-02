#ifndef BLOBS_SRC_WIN32API_MISC_H
#define BLOBS_SRC_WIN32API_MISC_H

#include "WinUtils.h"
DWORD GetTickCount();
DWORD timeGetTime();
bool GetUserNameA(
    /*out*/ char *lpBuffer,
    /*in, out*/ DWORD *pcbBuffer
);

#endif //BLOBS_SRC_WIN32API_MISC_H
