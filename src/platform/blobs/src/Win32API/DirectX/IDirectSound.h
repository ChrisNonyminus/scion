#ifndef BLOBS_SRC_WIN32API_DIRECTX_IDIRECTSOUND_H
#define BLOBS_SRC_WIN32API_DIRECTX_IDIRECTSOUND_H

#include "GZTypes.h"
#include "Win32API/WinUtils.h"

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#endif

typedef struct WAVEFORMATEX {
  uint16_t wFormatTag;
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
  uint16_t cbSize;
} WAVEFORMATEX;

typedef struct DSBUFFERDESC {
  uint32_t dwSize;
  uint32_t dwFlags;
  uint32_t dwBufferBytes;
  uint32_t dwReserved;
  WAVEFORMATEX *lpwfxFormat;
  uint8_t guid3DAlgorithm[0x20];
} DSBUFFERDESC;

struct dsndbuffer_ext {
  uint32_t lpVtbl;
  uint8_t padding[0x200];
  uint8_t *buffer;
  uint32_t size;
  int channel;
  int volume;

  WAVEFORMATEX format;
  void *raw_buffer;
};

#define DSCAPS_PRIMARYMONO          0x00000001
#define DSCAPS_PRIMARYSTEREO        0x00000002
#define DSCAPS_PRIMARY8BIT          0x00000004
#define DSCAPS_PRIMARY16BIT         0x00000008
#define DSCAPS_CONTINUOUSRATE       0x00000010
#define DSCAPS_EMULDRIVER           0x00000020
#define DSCAPS_CERTIFIED            0x00000040
#define DSCAPS_SECONDARYMONO        0x00000100
#define DSCAPS_SECONDARYSTEREO      0x00000200
#define DSCAPS_SECONDARY8BIT        0x00000400
#define DSCAPS_SECONDARY16BIT       0x00000800
typedef struct DSCAPS {
  DWORD dwSize;
  DWORD dwFlags;
  DWORD dwMinSecondarySampleRate, dwMaxSecondarySampleRate;
  DWORD dwPrimaryBuffers;
  DWORD dwMaxHwMixingAllBuffers;
  DWORD dwMaxHwMixingStaticBuffers;
  DWORD dwMaxHwMixingStreamingBuffers;
  DWORD dwFreeHwMixingAllBuffers;
  DWORD dwFreeHwMixingStaticBuffers;
  DWORD dwFreeHwMixingStreamingBuffers;
  DWORD dwMaxHw3DAllBuffers;
  DWORD dwMaxHw3DStaticBuffers;
  DWORD dwMaxHw3DStreamingBuffers;
  DWORD dwFreeHw3DAllBuffers;
  DWORD dwFreeHw3DStaticBuffers;
  DWORD dwFreeHw3DStreamingBuffers;
  DWORD dwTotalHwMemBytes;
  DWORD dwFreeHwMemBytes;
  DWORD dwMaxContigFreeHwMemBytes;
  DWORD dwUnlockTransferRateHwBuffers;
  DWORD dwPlayCpuOverheadSwBuffers;
  DWORD dwReserved1;
  DWORD dwReserved2;
} DSCAPS;

namespace Win32API {

class IDirectSound : public IUnknown {
public:
  virtual uint32_t CreateSoundBuffer(DSBUFFERDESC *pcDSBufferDesc, void *
  *ppDSBuffer, void *pUnkOuter) = 0;
  virtual uint32_t GetCaps(DSCAPS *pDSCaps) = 0;
  virtual uint32_t DuplicateSoundBuffer(void *pDSBufferOriginal,
                                        void **ppDSBufferDuplicate) = 0;
  virtual uint32_t SetCooperativeLevel(uint32_t hwnd, uint32_t dwLevel) = 0;
  virtual uint32_t Compact() = 0;
  virtual uint32_t GetSpeakerConfig(uint32_t *pdwSpeakerConfig) = 0;
  virtual uint32_t SetSpeakerConfig(uint32_t dwSpeakerConfig) = 0;
  virtual uint32_t Initialize(uint8_t *pcGuidDevice) = 0;
};

class IDirectSoundBuffer : public IUnknown {
public:
  virtual int32_t GetCaps(void *pDSBufferCaps) = 0;
  virtual uint32_t GetCurrentPosition(uint32_t *pdwCurrentPlayCursor,
                                      uint32_t *pdwCurrentWriteCursor) = 0;
  virtual uint32_t GetFormat(WAVEFORMATEX *pwfxFormat,
                             uint32_t dwSizeAllocated,
                             uint32_t *pdwSizeWritten) = 0;
  virtual uint32_t GetVolume(int32_t *plVolume) = 0;
  virtual uint32_t GetPan(int32_t *plPan) = 0;
  virtual uint32_t GetFrequency(uint32_t *pdwFrequency) = 0;
  virtual uint32_t GetStatus(uint32_t *pdwStatus) = 0;
  virtual uint32_t Initialize(uint32_t pDirectSound,
                              DSBUFFERDESC *pcDSBufferDesc) = 0;
  virtual uint32_t Lock(uint32_t dwOffset,
                        uint32_t dwBytes,
                        void **ppvAudioPtr1,
                        uint32_t *pdwAudioBytes1,
                        void **ppvAudioPtr2,
                        uint32_t *pdwAudioBytes2,
                        uint32_t dwFlags) = 0;
  virtual uint32_t Play(uint32_t dwReserved1,
                        uint32_t dwPriority,
                        uint32_t dwFlags) = 0;
  virtual uint32_t SetCurrentPosition(uint32_t dwNewPosition) = 0;
  virtual uint32_t SetFormat(WAVEFORMATEX *pcfxFormat) = 0;
  virtual uint32_t SetVolume(int32_t lVolume) = 0;
  virtual uint32_t SetPan(int32_t lPan) = 0;
  virtual uint32_t SetFrequency(uint32_t dwFrequency) = 0;
  virtual uint32_t Stop() = 0;
  virtual uint32_t Unlock(void *pvAudioPtr1,
                          uint32_t dwAudioBytes1,
                          void *pvAudioPtr2,
                          uint32_t dwAudioBytes2) = 0;
  virtual uint32_t Restore() = 0;
};

struct CDirectSound : public IDirectSound {
  uint32_t QueryInterface(uint8_t *guid, void **ppvObject);
  uint32_t AddRef();
  uint32_t Release();
  uint32_t CreateSoundBuffer(DSBUFFERDESC *pcDSBufferDesc,
                             void **ppDSBuffer,
                             void *pUnkOuter);
  uint32_t GetCaps(DSCAPS *pDSCaps);
  uint32_t DuplicateSoundBuffer(void *pDSBufferOriginal,
                                void **ppDSBufferDuplicate);
  uint32_t SetCooperativeLevel(uint32_t hwnd, uint32_t dwLevel);
  uint32_t Compact();
  uint32_t GetSpeakerConfig(uint32_t *pdwSpeakerConfig);
  uint32_t SetSpeakerConfig(uint32_t dwSpeakerConfig);
  uint32_t Initialize(uint8_t *pcGuidDevice);

  uint32_t ref_count;

  CDirectSound();
  ~CDirectSound();

#ifdef __linux__
  SDL_AudioSpec spec;
  SDL_AudioDeviceID device;
  SDL_AudioSpec obtained;
#endif
  static bool bInitialized;

  WAVEFORMATEX format;
};

struct CDirectSoundBuffer : public IDirectSoundBuffer {
  uint32_t QueryInterface(uint8_t *guid, void **ppvObject);
  uint32_t AddRef();
  uint32_t Release();
  int32_t GetCaps(void *pDSBufferCaps);
  uint32_t GetCurrentPosition(uint32_t *pdwCurrentPlayCursor,
                              uint32_t *pdwCurrentWriteCursor);
  uint32_t GetFormat(WAVEFORMATEX *pwfxFormat,
                     uint32_t dwSizeAllocated,
                     uint32_t *pdwSizeWritten);
  uint32_t GetVolume(int32_t *plVolume);
  uint32_t GetPan(int32_t *plPan);
  uint32_t GetFrequency(uint32_t *pdwFrequency);
  uint32_t GetStatus(uint32_t *pdwStatus);
  uint32_t Initialize(uint32_t pDirectSound, DSBUFFERDESC *pcDSBufferDesc);
  uint32_t Lock(uint32_t dwOffset,
                uint32_t dwBytes,
                void **ppvAudioPtr1,
                uint32_t *pdwAudioBytes1,
                void **ppvAudioPtr2,
                uint32_t *pdwAudioBytes2,
                uint32_t dwFlags);
  uint32_t Play(uint32_t dwReserved1, uint32_t dwPriority, uint32_t dwFlags);
  uint32_t SetCurrentPosition(uint32_t dwNewPosition);
  uint32_t SetFormat(WAVEFORMATEX *pcfxFormat);
  uint32_t SetVolume(int32_t lVolume);
  uint32_t SetPan(int32_t lPan);
  uint32_t SetFrequency(uint32_t dwFrequency);
  uint32_t Stop();
  uint32_t Unlock(void *pvAudioPtr1,
                  uint32_t dwAudioBytes1,
                  void *pvAudioPtr2,
                  uint32_t dwAudioBytes2);
  uint32_t Restore();

  uint32_t ref_count;

  CDirectSoundBuffer(DSBUFFERDESC *pcDSBufferDesc);
  ~CDirectSoundBuffer();
  CDirectSoundBuffer(CDirectSoundBuffer *other);

  void FillBuffer(uint8_t *stream, uint32_t len);

  void *buffer;
  uint32_t buffer_size;
  uint32_t buffer_play_cursor;
  uint32_t buffer_write_cursor;
  int32_t volume;
  int32_t pan;
  uint32_t frequency;
  uint32_t status;

#ifdef __linux__
  SDL_AudioDeviceID device;
#endif

  WAVEFORMATEX format;
};
} // Win32API

#endif //BLOBS_SRC_WIN32API_DIRECTX_IDIRECTSOUND_H
