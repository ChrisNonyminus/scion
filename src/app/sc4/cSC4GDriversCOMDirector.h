//
// Created by chris on 1/17/2023.
//

#ifndef SCION_CSC4GDRIVERSCOMDIRECTOR_H
#define SCION_CSC4GDRIVERSCOMDIRECTOR_H

#include "cRZCOMDllDirector.h"

struct cSC4GDriversCOMDirector : public cRZCOMDllDirector {
    cSC4GDriversCOMDirector();
    virtual uint32_t GetDirectorID() const;
    virtual bool InitializeFrameWork();
    ~cSC4GDriversCOMDirector() override;
};

cSC4GDriversCOMDirector* GetSC4GDriversCOMDirector();

static const uint32_t GZCLSID_cSC4GDriversCOMDirector = 0x64B2A903;

#endif //SCION_CSC4GDRIVERSCOMDIRECTOR_H
