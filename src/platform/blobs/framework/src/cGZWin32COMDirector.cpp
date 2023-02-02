//
// Created by chris on 1/17/2023.
//

#include "cGZWin32COMDirector.h"

cGZWin32COMDirector *GetGZWin32COMDllDirector() {
    static cGZWin32COMDirector director;
    return &director;
}

cGZWin32COMDirector::cGZWin32COMDirector() {
    AddCls(0x84174448, cGZWin32COMDirector::CreateGZWinMgr);

}

cGZWin32COMDirector::~cGZWin32COMDirector() {

}

cIGZUnknown *cGZWin32COMDirector::CreateGZWinMgr() {
#ifdef USING_SDL2
    return new cGZWinMgrSDL2(); //
}
