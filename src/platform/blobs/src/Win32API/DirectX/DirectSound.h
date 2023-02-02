#ifndef BLOBS_SRC_WIN32API_DIRECTX_DIRECTSOUND_H
#define BLOBS_SRC_WIN32API_DIRECTX_DIRECTSOUND_H

#include "GZTypes.h"
#include "Win32API/WinUtils.h"

#include "IDirectSound.h"

uint32_t DirectSoundCreate(uint8_t *lpGuid, Win32API::IDirectSound **ppDS);

#endif //BLOBS_SRC_WIN32API_DIRECTX_DIRECTSOUND_H
