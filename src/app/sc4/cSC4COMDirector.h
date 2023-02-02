//
// Created by chris on 1/17/2023.
//

#ifndef SCION_CSC4COMDIRECTOR_H
#define SCION_CSC4COMDIRECTOR_H

#include "cRZCOMDllDirector.h"

struct cSC4COMDirector : public cRZCOMDllDirector {
    cSC4COMDirector();
    virtual uint32_t GetDirectorID() const;
    virtual bool InitializeFrameWork();
    ~cSC4COMDirector() override;
};

static const uint32_t GZCLSID_cSC4COMDirector = 0x63CC0AF8;

#endif //SCION_CSC4COMDIRECTOR_H
