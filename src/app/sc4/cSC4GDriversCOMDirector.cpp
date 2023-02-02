//
// Created by chris on 1/17/2023.
//

#include "cSC4GDriversCOMDirector.h"

#include "cSGLDriverDX7.h"

cSC4GDriversCOMDirector::cSC4GDriversCOMDirector() {
    AddCls(GZCOM_cSGLDriverDX7CLSID(), GZCOM_CreatecSGLDriverDX7);

}

uint32_t cSC4GDriversCOMDirector::GetDirectorID() const {
    return 0x64B2A903;
}

bool cSC4GDriversCOMDirector::InitializeFrameWork() {
    return false;
}

cSC4GDriversCOMDirector::~cSC4GDriversCOMDirector() {

}

cSC4GDriversCOMDirector *GetSC4GDriversCOMDirector() {
    static cSC4GDriversCOMDirector director;
    return &director;
}
