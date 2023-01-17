//
// Created by chris on 1/17/2023.
//

#include "cSC4App.h"
#include "cIGZMessageTarget2.h"
#include "cGZFramework.h"
#include "RZStatics.h"
#include "cRZString.h"

#include "SL.h"
#include "GZPersist.h"
#include "cSC4AutoLoadRecord.h"
#include "SC4Statics.h"


cSC4App::cSC4App(const char *szName) : cGZApp(szName) {
    m_Timer1.Start();
    m_Timer2.Start();
    m_Timer3.Start();
    m_Timer4.Start();
    m_Timer5.Start();
    m_Prefs = new cSC4Preferences;
    SL::OnAppCreate(this);
}
uint32_t cSC4App::AddRef() { return cGZApp::AddRef(); }
uint32_t cSC4App::Release() { return cGZApp::Release(); }
bool cSC4App::QueryInterface(uint32_t uiClsId, void **ppvObj) {
    switch (uiClsId) {
        case 0xE98B2F57:
        case 0xCA263481:
            *ppvObj = this;
            break;
        /*case 0x452294AA:
            *ppvObj = static_cast<cIGZMessageTarget2 *>(this);
            break;
        case 0xC6AE7085:
            *ppvObj = static_cast<cIGZMessageFilter *>(this);
            break;*/
            /*case 0x26CE01C0:
                *ppvObj = static_cast<cISC4App*>(this);
                break;*/
        default:
            return cGZApp::QueryInterface(uiClsId, ppvObj);
    }
    AddRef();
    return true;
}
bool cSC4App::Init() {
    cGZFramework *pFramework = cGZFramework::mpFramework;
    return true;
}
bool cSC4App::RegisterShutdownCallbackFunction(
        cSC4App::ShutdownCallback fpCb, void *pvUserData) {
    ShutdownCallbackData data = {fpCb, pvUserData};
    m_ShutdownCallbackData.push_back(data);
    return true;
}
bool cSC4App::PreFrameWorkShutDown() {
    if (m_FCN.IsStarted()) m_FCN.Stop();
    m_FCN.Shutdown();
    if (m_FCN2.IsStarted()) m_FCN2.Stop();
    m_FCN2.Shutdown();
    CloseCity();
    ShutdownWinCtrlMgr();
    SavePreferences();
    /*if ( this->field_470 )
{
  (*(*this->field_470 + 16))(this->field_470);
  cRZAutoRefCount<cRZNetCommandGenerator>::operator=(
    &this->field_470,
    0);
}
if ( this->field_464 )
{
  (*(*this->field_464 + 32))(this->field_464);
  cRZAutoRefCount<cGZWinCommandSystemSupport>::operator=(
    &this->field_464,
    0);
}
if ( this->field_468 )
{
  (*(*this->field_468 + 32))(this->field_468);
  cRZAutoRefCount<cGZToolCommandSystemSupport>::operator=(
    &this->field_468,
    0);
}
if ( !this->field_46C )
  return 1;
(*(*this->field_46C + 32))(this->field_46C);
cRZAutoRefCount<cGZMiscCommandSystemSupport>::operator=(
  &this->field_46C,
  0);*/ // TODO
    return true;
}
void cSC4App::CloseCity() {}
void cSC4App::ShutdownWinCtrlMgr() {}
void cSC4App::ReadPreferences() {
    cGZPersistDBSegmentPackedFile *dbpf =
            new cGZPersistDBSegmentPackedFile;
    if (!dbpf->Init()) {
        dbpf->Release();
        return;
    }
    cRZString dbpfPath;
    GetAppPreferencesFilePath(dbpfPath);
    dbpf->SetPath(dbpfPath);
    if (!dbpf->Open(true, false)) {
        dbpf->Release();
        return;
    }
    cSC4AutoLoadRecord record(dbpf, 0xA9DD6E06, 0x49DD6E08, 1);
    if (record.m_pRecord) {
        if (record.field_C == 21) {
            cSC4Preferences tmpPrefs;
            if (record.m_pRecord->GetFieldVoid(&tmpPrefs,
                                               sizeof(cSC4Preferences))) {
                memcpy(m_Prefs, &tmpPrefs, sizeof(cSC4Preferences));
                field_494 = 0;
            }
        }
        record.Close();
    }
    dbpf->Close();
    dbpf->Release();
}
void cSC4App::SavePreferences() {}
void cSC4App::SetFontAlias(const cRZString &a2,
                                const cRZString &a3) {
    //RZ::FO()->AddSubstitution(a2, a3);
}
void cSC4App::SetFontAliasStatic(const cRZString &a1,
                                      const cRZString &a2, void *a3) {
    //RZ::FO()->AddSubstitution(a1, a2);
}
void cSC4App::SetFontStyle(const cRZString &a2,
                                const cRZString &a3) {
    // TODO
}
void cSC4App::SetFontStyleStatic(const cRZString &a1,
                                      const cRZString &a2, void *a3) {
    // TODO
}
void cSC4App::EnableFullGamePauseOnAppFocusLoss(bool v) {
    m_Prefs->m_AppPrefs.bPauseOnAppFocusLoss = v;
    m_bPauseOnAppFocusLoss = v;
}
void cSC4App::GainLoseFocus(bool v) {
    if (!m_bPauseOnAppFocusLoss) return;
    if (!v != m_bFullGamePaused) return FullGamePause(!v);
}
void cSC4App::FullGamePause(bool v) {
    if (m_bFullGamePaused == v) return;
    SC4GamePause(v, true, true, true, true);
}
void cSC4App::GetAppPreferencesFilePath(cRZString &dest) {
    cRZString filename;
    GetAppPreferencesFileName(filename);
    dest.FromChar(field_C8.ToChar());
    dest.Append(filename);
}
void cSC4App::GetAppPreferencesFileName(cRZString &dest) {
    if (field_5C.empty())
        dest.FromChar("SimCity 4.cfg");
    else {
        dest.FromChar(field_5C.c_str());
        dest.Append(cRZString(".cfg"));
    }
}
bool cSC4App::AddCOMDirectors() {
    cRZCOMDllDirector *director = RZGetCOMDllDirector();
    cIGZCOM *gzcom = GZCOM();
    return false;
}

bool cSC4App::DoMessage(const cGZMessage &msg) {
    return false;
}
