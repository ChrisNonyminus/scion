//
// Created by chris on 1/17/2023.
//

#include "cRZTimer.h"

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <sys/time.h>
#elif defined(__APPLE__)
#include <mach/mach_time.h>
#endif

cRZTimer::cRZTimer(cRZTimer::Units eUnits) : m_eUnits(eUnits) {}
void cRZTimer::Start() {
    // get current milliseconds and set m_uiTime to it
#ifdef _WIN32
    SYSTEMTIME st;
    GetSystemTime(&st);
    m_uiTime = st.wMilliseconds;
#elif defined(__linux__)
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    m_uiTime = tv.tv_usec / 1000;
#elif defined(__APPLE__)
    m_uiTime = mach_absolute_time() / 1000000;
#endif
}