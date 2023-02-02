#include "DirectSound.h"

uint32_t DirectSoundCreate(uint8_t *lpGuid, Win32API::IDirectSound **ppDS) {
  *ppDS = new Win32API::CDirectSound();
  return 0;
}
