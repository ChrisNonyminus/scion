#include "cGZGraphicCOMDirector.h"
#include "cGZGraphicWin32COMDirector.h"
#include "RZStatics.h"
#include "cRZSystemService.h"
#include "cIGZCOM.h"
#include "cIGZFramework.h"

cGZGraphicCOMDirector::cGZGraphicCOMDirector() {
    AddDirector(GetGZGraphicWin32COMDllDirector());


}

uint32_t cGZGraphicCOMDirector::GetDirectorID() const {
    return 0x246B917A;
}


bool cGZGraphicCOMDirector::OnStart(cIGZCOM *com) {
    cIGZSystemService* a2;
    GZCOM()->GetClassObject(0xA499932C, 0x287FB697u, (void**)&a2); // cGZGraphicSystemW32
    if (a2) {
        RZGetFramework()->AddSystemService(a2);
        a2->Release();
        a2 = nullptr;
    }
    GZCOM()->GetClassObject(0x445FABBBu, 0x287FB697u, (void**)&a2); // cGZCanvasW32
    if (a2) {
        RZGetFramework()->AddSystemService(a2);
        a2->Release();
        a2 = nullptr;
    }

    return true;
}
