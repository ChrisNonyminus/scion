#ifndef BLOBS_SRC_WIN32API_THREADING_H
#define BLOBS_SRC_WIN32API_THREADING_H


// Atomic/threading shit

#include "GZTypes.h"
#include "WinUtils.h"

enum EXECUTION_STATE {
  ES_AWAYMODE_REQUIRED = 0x00000040,
  ES_CONTINUOUS = 0x80000000,
  ES_DISPLAY_REQUIRED = 0x00000002,
  ES_SYSTEM_REQUIRED = 0x00000001,
  ES_USER_PRESENT = 0x00000004,
  ES_NONE = 0
};

int32_t InterlockedExchangeAdd(volatile int32_t *addend /*in, out*/,
                               int32_t value /*in*/);
DWORD GetCurrentThreadId();

/*return value: previous execution state*/
EXECUTION_STATE SetThreadExecutionState(
    /*in*/ EXECUTION_STATE esFlags
);


// increments the pointed-to value by 1 atomically
extern "C" void InterlockedIncrement(int *dest);
// decrements the pointed-to value by 1 atomically
extern "C" void InterlockedDecrement(int *dest);

#endif //BLOBS_SRC_WIN32API_THREADING_H
