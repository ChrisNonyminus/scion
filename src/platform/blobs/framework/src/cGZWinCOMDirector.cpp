//
// Created by chris on 1/17/2023.
//

#include "cGZWinCOMDirector.h"
#include "RZStatics.h"
#include "cGZFramework.h"

cGZWinCOMDirector::cGZWinCOMDirector() {

}

bool cGZWinCOMDirector::InitializeFrameWork() {
    return false;
}

cGZWinCOMDirector::~cGZWinCOMDirector() {

}

bool cGZWinCOMDirector::OnStart(cIGZCOM *com) {
    RZGetFramework()->AddHook(this);
    cIGZSystemService* srv;
    com->GetClassObject(RZSRVID_cGZWinCtrlMgr, GZIID_cIGZWinCtrlMgr, (void**)&srv);
    RZGetFramework()->AddSystemService(srv);
}

bool cGZWinCOMDirector::PostSystemServiceShutdown(void) {
    return cRZCOMDllDirector::PostSystemServiceShutdown();
}
