//
// Created by chris on 1/17/2023.
//

#ifndef SCION_CGZWINCOMDIRECTOR_H
#define SCION_CGZWINCOMDIRECTOR_H

#include "GZWin.h"
#include "cRZCOMDllDirector.h"

class cGZWinCOMDirector : public cRZCOMDllDirector {
public:
    cGZWinCOMDirector();
    virtual uint32_t GetDirectorID() const { return 0xA41740D1; }
    virtual bool InitializeFrameWork();
    ~cGZWinCOMDirector() override;

    bool OnStart(cIGZCOM *com) override;

    bool PostSystemServiceShutdown(void) override;


};

cGZWinCOMDirector* GetGZWinCOMDllDirector();

#endif //SCION_CGZWINCOMDIRECTOR_H
