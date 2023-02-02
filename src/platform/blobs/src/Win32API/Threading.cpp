#include "Threading.h"

#include <pthread.h>

int32_t InterlockedExchangeAdd(volatile int32_t *addend /*in, out*/,
                               int32_t value /*in*/) {
  int32_t result = *addend;
  *addend += value;
  return result;
}
DWORD GetCurrentThreadId() {
  return pthread_self();
}
EXECUTION_STATE SetThreadExecutionState(EXECUTION_STATE esFlags) {
  uint32_t prev = 0;
  if (esFlags & ES_CONTINUOUS) {
    prev |= ES_CONTINUOUS;
  }
  if (esFlags & ES_DISPLAY_REQUIRED) {
    prev |= ES_DISPLAY_REQUIRED;
  }
  if (esFlags & ES_SYSTEM_REQUIRED) {
    prev |= ES_SYSTEM_REQUIRED;
  }

  printf("SetThreadExecutionState(%d) = %d\n", esFlags, prev);
  return static_cast<EXECUTION_STATE>(prev);
}
void InterlockedIncrement(int *dest) {
  *dest += 1;
}
void InterlockedDecrement(int *dest) {
  *dest -= 1;
}
