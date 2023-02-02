//
// Created by chris on 1/17/2023.
//

#ifndef SCION_CGZWIN32COMDIRECTOR_H
#define SCION_CGZWIN32COMDIRECTOR_H

#include "GZWin.h"
#include "cRZCOMDllDirector.h"

class cGZWin32COMDirector : public cRZCOMDllDirector {
public:
    cGZWin32COMDirector();
    virtual uint32_t GetDirectorID() const { return 0x41740BA; }
    ~cGZWin32COMDirector() ;

    static cIGZUnknown* CreateGZWinMgr();

};

cGZWin32COMDirector* GetGZWin32COMDllDirector();

#endif //SCION_CGZWIN32COMDIRECTOR_H
