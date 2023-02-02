//
// Created by chris on 1/17/2023.
//

#ifndef SCION_CSC4SCRIPTEDWINDOWCOMDIRECTOR_H
#define SCION_CSC4SCRIPTEDWINDOWCOMDIRECTOR_H

#include "cRZCOMDllDirector.h"

struct cSC4ScriptedWindowCOMDirector : public cRZCOMDllDirector {
    cSC4ScriptedWindowCOMDirector();
    virtual uint32_t GetDirectorID() const;
    virtual bool InitializeFrameWork();
    ~cSC4ScriptedWindowCOMDirector() override;

    // FactoryFunctionPtr1 functions below
    static cIGZUnknown* CreateSC4WinMapView();
    static cIGZUnknown* CreateSC4WinLotConfigurationChooser();
    static cIGZUnknown* CreateSC4WinLotConfigurationEditor();
    static cIGZUnknown* CreateSC4WinBuildingFamilyChooser();
    static cIGZUnknown* CreateSC4WinBuildingChooser();
    static cIGZUnknown* CreateSC4WinNetworkLotChooser();
    static cIGZUnknown* CreateSC4WinNetworkLotEditor();
    static cIGZUnknown* CreateSC4WinRCI();
    static cIGZUnknown* CreateSC4WinTrendBar();
    static cIGZUnknown* CreateSC4WinGenTransparent();
    static cIGZUnknown* CreateSC4WinIMECandidateList();
    static cIGZUnknown* CreateGZWinKnob();
};

cSC4ScriptedWindowCOMDirector* SC4GetSC4ScriptedWindowCOMDirector();

static const uint32_t GZCLSID_cSC4ScriptedWindowCOMDirector = 0xE8C5B006;

#endif //SCION_CSC4SCRIPTEDWINDOWCOMDIRECTOR_H
