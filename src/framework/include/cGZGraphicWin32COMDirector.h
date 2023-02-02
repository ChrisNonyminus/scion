//
// Created by chris on 1/18/2023.
//

#ifndef SCION_CGZGRAPHICWIN32COMDIRECTOR_H
#define SCION_CGZGRAPHICWIN32COMDIRECTOR_H

#include "cRZCOMDllDirector.h"

struct cGZGraphicWin32COMDirector : public cRZCOMDllDirector {
    cGZGraphicWin32COMDirector();
    virtual uint32_t GetDirectorID() const;
    ~cGZGraphicWin32COMDirector() override = default;

    static cIGZUnknown* CreatecGZGraphicSystemWin32();
    static cIGZUnknown* CreatecGZCanvasW32();
};

#endif //SCION_CGZGRAPHICWIN32COMDIRECTOR_H
