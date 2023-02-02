#include <csignal>
#include "IDirectSound.h"

uint32_t Win32API::CDirectSound::QueryInterface(uint8_t *guid,
                                                void **ppvObject) {
  *ppvObject = this;
  return 0;
}
uint32_t Win32API::CDirectSound::AddRef() {
  return ref_count++;
}
uint32_t Win32API::CDirectSound::Release() {
  int ret = ref_count--;
  if (ret == 0) {
    delete this;
  }
  return ret;
}
bool Win32API::CDirectSound::bInitialized;
uint32_t Win32API::CDirectSound::CreateSoundBuffer(DSBUFFERDESC *pcDSBufferDesc,
                                                   void **ppDSBuffer,
                                                   void *pUnkOuter) {
  /*TODO: fix weird crashes. For now I'll just stub this func out*/
  return -1;

  *ppDSBuffer = new Win32API::CDirectSoundBuffer(pcDSBufferDesc);
  ((Win32API::CDirectSoundBuffer *) (*ppDSBuffer))->AddRef();
  if (pcDSBufferDesc->lpwfxFormat != NULL)
    format = *pcDSBufferDesc->lpwfxFormat;
  else {
    format.nSamplesPerSec = 44100;
    format.nChannels = 2;
  }
  ((Win32API::CDirectSoundBuffer *) (*ppDSBuffer))->format = format;
  spec.freq = format.nSamplesPerSec;
  spec.format = AUDIO_S16;
  spec.channels = format.nChannels;
  spec.samples = 4096;
  spec.callback = [](void *userdata, uint8_t *stream, int len) {
    Win32API::CDirectSoundBuffer
        *buffer = (Win32API::CDirectSoundBuffer *) userdata;
    buffer->FillBuffer(stream, len);
  };
  spec.userdata = *ppDSBuffer;

  if (!bInitialized)
    device = SDL_OpenAudio(&spec, &spec);
  if (device != 0) {
    printf("Failed to open audio: %s\n", SDL_GetError());
    raise(SIGSEGV);
  }
  ((Win32API::CDirectSoundBuffer *) (*ppDSBuffer))->device = device;
  SDL_PauseAudio(0);
  bInitialized = true;

  return 0;
}
uint32_t Win32API::CDirectSound::GetCaps(DSCAPS *pDSCaps) {
  DSCAPS &caps = (*pDSCaps);
  caps.dwFlags = DSCAPS_PRIMARY16BIT | DSCAPS_PRIMARYSTEREO;
  caps.dwMinSecondarySampleRate = 8000;
  caps.dwMaxSecondarySampleRate = 48000;
  caps.dwPrimaryBuffers = 1;
  caps.dwMaxHwMixingAllBuffers = 0;
  caps.dwMaxHwMixingStaticBuffers = 0;
  caps.dwMaxHwMixingStreamingBuffers = 0;
  caps.dwFreeHwMixingAllBuffers = 0;
  caps.dwFreeHwMixingStaticBuffers = 0;
  caps.dwFreeHwMixingStreamingBuffers = 0;
  caps.dwMaxHw3DAllBuffers = 0;
  caps.dwMaxHw3DStaticBuffers = 0;
  caps.dwMaxHw3DStreamingBuffers = 0;
  caps.dwFreeHw3DAllBuffers = 0;
  caps.dwFreeHw3DStaticBuffers = 0;
  caps.dwFreeHw3DStreamingBuffers = 0;
  caps.dwTotalHwMemBytes = 0;

  return 0;
}
uint32_t Win32API::CDirectSound::DuplicateSoundBuffer(void *pDSBufferOriginal,
                                                      void **ppDSBufferDuplicate) {
  Win32API::CDirectSoundBuffer
      *buffer = (Win32API::CDirectSoundBuffer *) pDSBufferOriginal;
  *ppDSBufferDuplicate = new Win32API::CDirectSoundBuffer(buffer);
  return 0;
}
/*This method sets the cooperative level of the application for this sound device.*/
uint32_t Win32API::CDirectSound::SetCooperativeLevel(uint32_t hwnd,
                                                     uint32_t dwLevel) {
  return 0;
}
/*This method moves the unused portions of on-board sound memory, if any, to a contiguous block so that the largest portion of free memory will be available.*/
uint32_t Win32API::CDirectSound::Compact() {
  return 0;
}
uint32_t Win32API::CDirectSound::GetSpeakerConfig(uint32_t *pdwSpeakerConfig) {
  return 0;
}
uint32_t Win32API::CDirectSound::SetSpeakerConfig(uint32_t dwSpeakerConfig) {
  return 0;
}
uint32_t Win32API::CDirectSound::Initialize(uint8_t *pcGuidDevice) {
  return 0;
}
Win32API::CDirectSound::CDirectSound() {
  ref_count = 1;

}
Win32API::CDirectSound::~CDirectSound() {
  SDL_CloseAudio();
  bInitialized = false;
}

uint32_t Win32API::CDirectSoundBuffer::QueryInterface(uint8_t *guid,
                                                      void **ppvObject) {
  *ppvObject = this;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::AddRef() {
  return ref_count++;
}
uint32_t Win32API::CDirectSoundBuffer::Release() {
  int ret = ref_count--;
  if (ret == 0) {
    delete this;
  }
  return ret;
}
int32_t Win32API::CDirectSoundBuffer::GetCaps(void *pDSBufferCaps) {
  return -1;
}
uint32_t Win32API::CDirectSoundBuffer::GetCurrentPosition(uint32_t *pdwCurrentPlayCursor,
                                                          uint32_t *pdwCurrentWriteCursor) {
  if (pdwCurrentPlayCursor != NULL)
    *pdwCurrentPlayCursor = buffer_play_cursor;
  if (pdwCurrentWriteCursor != NULL)
    *pdwCurrentWriteCursor = buffer_write_cursor;
  return -1;
}
uint32_t Win32API::CDirectSoundBuffer::GetFormat(WAVEFORMATEX *pwfxFormat,
                                                 uint32_t dwSizeAllocated,
                                                 uint32_t *pdwSizeWritten) {
  *pwfxFormat = format;
  *pdwSizeWritten = sizeof(WAVEFORMATEX);
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::GetVolume(int32_t *plVolume) {
  *plVolume = volume;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::GetPan(int32_t *plPan) {
  *plPan = pan;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::GetFrequency(uint32_t *pdwFrequency) {
  *pdwFrequency = frequency;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::GetStatus(uint32_t *pdwStatus) {
  *pdwStatus = status;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::Initialize(uint32_t pDirectSound,
                                                  DSBUFFERDESC *pcDSBufferDesc) {
  if (pcDSBufferDesc->lpwfxFormat != NULL)
    format = *pcDSBufferDesc->lpwfxFormat;
  buffer_size = pcDSBufferDesc->dwBufferBytes;
  buffer = new uint8_t[buffer_size];
  buffer_play_cursor = 0;
  buffer_write_cursor = 0;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::Lock(uint32_t dwOffset,
                                            uint32_t dwBytes,
                                            void **ppvAudioPtr1,
                                            uint32_t *pdwAudioBytes1,
                                            void **ppvAudioPtr2,
                                            uint32_t *pdwAudioBytes2,
                                            uint32_t dwFlags) {
  SDL_LockAudio();
  /**ppvAudioPtr1 = buffer + dwOffset;
  *pdwAudioBytes1 = dwBytes;
  *ppvAudioPtr2 = NULL;
  *pdwAudioBytes2 = 0;*/
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::Play(uint32_t dwReserved1,
                                            uint32_t dwPriority,
                                            uint32_t dwFlags) {
  status = 1;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::SetCurrentPosition(uint32_t dwNewPosition) {
  buffer_play_cursor = dwNewPosition;
  buffer_write_cursor = dwNewPosition;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::SetFormat(WAVEFORMATEX *pcfxFormat) {
  format = *pcfxFormat;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::SetVolume(int32_t lVolume) {
  volume = lVolume;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::SetPan(int32_t lPan) {
  pan = lPan;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::SetFrequency(uint32_t dwFrequency) {
  frequency = dwFrequency;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::Stop() {
  status = 0;
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::Unlock(void *pvAudioPtr1,
                                              uint32_t dwAudioBytes1,
                                              void *pvAudioPtr2,
                                              uint32_t dwAudioBytes2) {
  SDL_UnlockAudio();
  return 0;
}
uint32_t Win32API::CDirectSoundBuffer::Restore() {
  return 0;
}
Win32API::CDirectSoundBuffer::CDirectSoundBuffer(DSBUFFERDESC *pcDSBufferDesc) {
  ref_count = 1;
  Initialize(NULL, pcDSBufferDesc);

}
Win32API::CDirectSoundBuffer::~CDirectSoundBuffer() {
  delete[] buffer;

}
Win32API::CDirectSoundBuffer::CDirectSoundBuffer(Win32API::CDirectSoundBuffer *other) {
  ref_count = 1;
  //Initialize(NULL, NULL);
  format = other->format;
  buffer_size = other->buffer_size;
  buffer = new uint8_t[buffer_size];
  memcpy(buffer, other->buffer, buffer_size);
  buffer_play_cursor = other->buffer_play_cursor;
  buffer_write_cursor = other->buffer_write_cursor;
  volume = other->volume;
  pan = other->pan;
  frequency = other->frequency;
  status = other->status;

}

// SDL audio callback
void Win32API::CDirectSoundBuffer::FillBuffer(uint8_t *stream, uint32_t len) {
  if (status == 0) {
    return;
  }
  uint32_t bytes_to_copy = len;
  if (buffer_write_cursor + bytes_to_copy > buffer_size) {
    bytes_to_copy = buffer_size - buffer_write_cursor;
  }
  memcpy(stream, (uint8_t *) buffer + buffer_write_cursor, bytes_to_copy);
  buffer_write_cursor += bytes_to_copy;
  if (buffer_write_cursor == buffer_size) {
    buffer_write_cursor = 0;
  }
  if (bytes_to_copy < len) {
    memcpy(stream + bytes_to_copy, (uint8_t *) buffer + buffer_write_cursor,
           len -
               bytes_to_copy);
    buffer_write_cursor += len - bytes_to_copy;
  }
}
