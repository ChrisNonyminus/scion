//
// Created by chris on 1/17/2023.
//

#include "cSC4ScriptedWindowCOMDirector.h"

#include "SC4Win.h"

cSC4ScriptedWindowCOMDirector::cSC4ScriptedWindowCOMDirector() {
    AddCls(GZCOM_cSC4WinAdviceListCLSID(), GZCOM_CreatecSC4WinAdviceList);
    AddCls(GZCOM_cSC4WinMiniMapCLSID(), GZCOM_CreatecSC4WinMiniMap);
    AddCls(GZCOM_cSC4WinAuraBarCLSID(), GZCOM_CreatecSC4WinAuraBar);
    AddCls(GZCOM_cSC4WinAlertBorderCLSID(), GZCOM_CreatecSC4WinAlertBorder);
    AddCls(GZCOM_cSC4WinTextCLSID(), GZCOM_CreatecSC4WinText);
}

uint32_t cSC4ScriptedWindowCOMDirector::GetDirectorID() const {
    return GZCLSID_cSC4ScriptedWindowCOMDirector;
}

bool cSC4ScriptedWindowCOMDirector::InitializeFrameWork() {
    return false;
}

cSC4ScriptedWindowCOMDirector::~cSC4ScriptedWindowCOMDirector() {

}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateSC4WinMapView() {
    return nullptr;
}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateSC4WinLotConfigurationChooser() {
    return nullptr;
}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateSC4WinLotConfigurationEditor() {
    return nullptr;
}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateSC4WinBuildingFamilyChooser() {
    return nullptr;
}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateSC4WinBuildingChooser() {
    return nullptr;
}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateSC4WinNetworkLotChooser() {
    return nullptr;
}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateSC4WinNetworkLotEditor() {
    return nullptr;
}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateSC4WinRCI() {
    return nullptr;
}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateSC4WinTrendBar() {
    return nullptr;
}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateSC4WinGenTransparent() {
    return nullptr;
}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateSC4WinIMECandidateList() {
    return nullptr;
}

cIGZUnknown *cSC4ScriptedWindowCOMDirector::CreateGZWinKnob() {
    return nullptr;
}

cSC4ScriptedWindowCOMDirector *SC4GetSC4ScriptedWindowCOMDirector() {
    static cSC4ScriptedWindowCOMDirector gDirector;
    return &gDirector;
}
