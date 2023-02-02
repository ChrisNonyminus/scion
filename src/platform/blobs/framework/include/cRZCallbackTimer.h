#ifndef BLOBS_FRAMEWORK_INCLUDE_CRZCALLBACKTIMER_H
#define BLOBS_FRAMEWORK_INCLUDE_CRZCALLBACKTIMER_H

#include "cIGZUnknown.h"

#include <SDL2/SDL.h>

class cRZCallbackTimer {
  uint8_t unk[0x8 - 4];
  void* m_pvCallback;
  union  {
    void* pv;
    uint32_t ul;
  }m_uCallbackArg;
  uint32_t m_ulResolution;
  uint32_t m_ulPeriod;
  bool field_18, m_bDoOneShot;
  uint8_t unk19[0x1C - 0x1A];
  SDL_TimerID m_timerID;
  cIGZUnknown* field_20;
public:
  static uint32_t InternalCallbackFunction(uint32_t interval, void* inTimer);
  static uint32_t InternalCallbackFunctionOneShot(uint32_t interval, void* inTimer);
  cRZCallbackTimer();
  virtual uint32_t SetResolution(uint32_t res);
  virtual uint32_t GetResolution(uint32_t& dst);
  virtual uint32_t SetPeriod(uint32_t res);
  virtual uint32_t GetPeriod(uint32_t& dst);
  virtual uint32_t StartPeriodic(uint32_t a2, cIGZUnknown* a3);
  virtual uint32_t StartOneShot(uint32_t a2, cIGZUnknown* a3);
  virtual uint32_t Stop();
  virtual bool IsRunning();
  virtual uint32_t SetCallbackFunction(void(*callback)(uint32_t), uint32_t userData);
  virtual uint32_t SetCallbackFunction(void(*callback)(void*), void* userData);
  virtual uint32_t EnableSharedTimerUse(bool val);
  uint32_t GetCurrentSystemMillesecondTime();
  virtual ~cRZCallbackTimer();
};

#endif //BLOBS_FRAMEWORK_INCLUDE_CRZCALLBACKTIMER_H
