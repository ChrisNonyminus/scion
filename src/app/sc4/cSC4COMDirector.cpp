//
// Created by chris on 1/17/2023.
//

#include "cSC4COMDirector.h"
#include "RZStatics.h"

#include "cSC4ScriptedWindowCOMDirector.h"
#include "cSC4GDriversCOMDirector.h"

cSC4COMDirector::cSC4COMDirector() {
    cRZCOMDllDirector* rootDirector = RZGetCOMDllDirector();
    if (rootDirector) {
        cIGZCOM* gzcom = ::GZCOM();
        auto* scriptedWindowDirector = SC4GetSC4ScriptedWindowCOMDirector();
        AddDirector(scriptedWindowDirector);
        scriptedWindowDirector->OnStart(gzcom);
        auto* gDriversDirector = GetSC4GDriversCOMDirector();
        AddDirector(gDriversDirector);
        gDriversDirector->OnStart(gzcom);
    }

}

uint32_t cSC4COMDirector::GetDirectorID() const {
    return GZCLSID_cSC4COMDirector;
}

bool cSC4COMDirector::InitializeFrameWork() {
    return false;
}

cSC4COMDirector::~cSC4COMDirector() {

}
