#include <ctime>
#include "cRZCallbackTimer.h"

uint32_t cRZCallbackTimer::SetResolution(uint32_t res) {
  cIGZUnknown* tmp = field_20;
  if (field_20)
    tmp->AddRef();
  m_ulResolution = res;
  if (field_18) {
    Stop();
    if (m_bDoOneShot)
      StartOneShot(0, NULL);
    else
      StartPeriodic(0, NULL);
  }
  if (!tmp) return 1;
  if (field_20) tmp->Release();
  field_20 = tmp;
  return 1;
}
uint32_t cRZCallbackTimer::GetResolution(uint32_t &dst) {
  dst = m_ulResolution;
  return 1;
}

uint32_t cRZCallbackTimer::SetPeriod(uint32_t res) {
  cIGZUnknown* tmp = field_20;
  if (field_20)
    tmp->AddRef();
  m_ulPeriod = res;
  if (field_18) {
    Stop();
    if (m_bDoOneShot)
      StartOneShot(0, NULL);
    else
      StartPeriodic(0, NULL);
  }
  if (!tmp) return 1;
  if (field_20) tmp->Release();
  field_20 = tmp;
  return 1;
}

uint32_t cRZCallbackTimer::GetPeriod(uint32_t &dst) {
  dst = m_ulPeriod;
  return 1;
}



uint32_t cRZCallbackTimer::StartPeriodic(uint32_t a2, cIGZUnknown *a3) {
  if (field_18 || !m_pvCallback)
    return 0;
  if (a2) m_ulPeriod = a2;
  if (a3) {
    a3->AddRef();
    field_20 = a3;
  }
  field_18 = true;
  // do timer stuff here
  m_timerID = SDL_AddTimer(m_ulPeriod,
                                     &cRZCallbackTimer::InternalCallbackFunction, this);
  if (!m_timerID) {
    printf("Error adding cRZCallbackTimer callback! SDL_AddTimer returned: \"%s\".", SDL_GetError());

    // do this if installing the timer fails
    field_18 = false;
    if (a3) a3->Release();
    field_20 = NULL;
    return 0;
  }
  return 1;

}

uint32_t cRZCallbackTimer::StartOneShot(uint32_t a2, cIGZUnknown *a3) {
  if (field_18 || !m_pvCallback)
    return 0;
  if (a2) m_ulPeriod = a2;
  if (a3) {
    a3->AddRef();
    field_20 = a3;
  }
  field_18 = true;
  m_bDoOneShot = true;
  // do timer stuff here
  m_timerID = SDL_AddTimer(m_ulPeriod,
                                     &cRZCallbackTimer::InternalCallbackFunctionOneShot, this);
  if (!m_timerID) {
    printf("Error adding cRZCallbackTimer callback! SDL_AddTimer returned: \"%s\".", SDL_GetError());

    // do this if installing the timer fails
    field_18 = false;
    m_bDoOneShot = false;
    if (a3) a3->Release();
    field_20 = NULL;
    return 0;
  }
  return 1;
}

uint32_t cRZCallbackTimer::Stop() {
  if (!m_timerID) return 1;
  if (field_20) {
    field_20->Release();
    field_20 = NULL;
  }
  m_bDoOneShot = false;
  field_18 = false;
  SDL_RemoveTimer(m_timerID);
  m_timerID = 0;
  return 1;
}

bool cRZCallbackTimer::IsRunning() {
  return field_18;
}

typedef void (*CallbackTakingInUlong)(uint32_t);
typedef void (*CallbackTakingInVoidPtr)(void*);

uint32_t cRZCallbackTimer::SetCallbackFunction(void (*callback)(uint32_t), uint32_t userData) {
  m_pvCallback = reinterpret_cast<void*>(callback);
  m_uCallbackArg.ul = userData;
  return 1;
}

uint32_t cRZCallbackTimer::SetCallbackFunction(void (*callback)(void *), void *userData) {
  m_pvCallback = reinterpret_cast<void*>(callback);
  m_uCallbackArg.pv = userData;
  return 1;
}

uint32_t cRZCallbackTimer::EnableSharedTimerUse(bool val) {
  return 1;
}
cRZCallbackTimer::cRZCallbackTimer() {
  m_pvCallback = NULL;
  m_ulResolution = 100;
  m_ulPeriod = 1000;
  field_18 = false;
  m_bDoOneShot = false;
  field_20 = NULL;
}
uint32_t cRZCallbackTimer::InternalCallbackFunction(uint32_t interval, void *inTimer) {
  cRZCallbackTimer* callbackTimer =
      reinterpret_cast<cRZCallbackTimer*>(inTimer);
  CallbackTakingInVoidPtr voidptrCallback =
      reinterpret_cast<CallbackTakingInVoidPtr>(callbackTimer->m_pvCallback);
  if (callbackTimer->field_20) {
    callbackTimer->field_20->AddRef();
  }
  voidptrCallback(callbackTimer->m_uCallbackArg.pv);
  if (callbackTimer->field_20) {
    callbackTimer->field_20->Release();
  }
  return interval;
}
uint32_t cRZCallbackTimer::InternalCallbackFunctionOneShot(uint32_t interval, void *inTimer) {
  cRZCallbackTimer* callbackTimer =
      reinterpret_cast<cRZCallbackTimer*>(inTimer);
  CallbackTakingInVoidPtr voidptrCallback =
      reinterpret_cast<CallbackTakingInVoidPtr>(callbackTimer->m_pvCallback);
  if (callbackTimer->field_20) {
    callbackTimer->field_20->AddRef();
  }

  if (callbackTimer->field_20) {
    callbackTimer->field_20->Release();
    callbackTimer->field_20 = NULL;
  }
  callbackTimer->field_18 = false;
  callbackTimer->m_bDoOneShot = false;

  voidptrCallback(callbackTimer->m_uCallbackArg.pv);
  return interval;
}
cRZCallbackTimer::~cRZCallbackTimer() {
  Stop();
  m_pvCallback = NULL;
  m_uCallbackArg.pv = NULL;
}
uint32_t cRZCallbackTimer::GetCurrentSystemMillesecondTime() {
  return clock();
}
