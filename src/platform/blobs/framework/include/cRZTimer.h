//
// Created by chris on 1/17/2023.
//

#ifndef SCION_CRZTIMER_H
#define SCION_CRZTIMER_H

#include "GZTypes.h"

struct cRZTimer {
    enum Units {
        TIMER_DIV_1000 = 3,
        TIMER_UNUSED_MAYBE1 = 4,
        TIMER_MUL_1000 = 5,
        TIMER_MUL_60000 = 6
    };
    Units m_eUnits;
    uint64_t m_uiTime = 0;
    int field_8 = 0;
    double m_dElapsedTime = 0.0;

    cRZTimer(Units eUnits);

    void Start();
};

#endif //SCION_CRZTIMER_H
