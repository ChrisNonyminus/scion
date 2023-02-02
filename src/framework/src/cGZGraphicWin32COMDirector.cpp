//
// Created by chris on 1/18/2023.
//

#include "cGZGraphicWin32COMDirector.h"
#include "cGZGraphicSystemWin32.h"
#include "cGZCanvasW32.h"

cGZGraphicWin32COMDirector::cGZGraphicWin32COMDirector() {
    AddCls(0xA499932C, cGZGraphicWin32COMDirector::CreatecGZGraphicSystemWin32);
    AddCls(0x445FABBB, cGZGraphicWin32COMDirector::CreatecGZCanvasW32);

}

uint32_t cGZGraphicWin32COMDirector::GetDirectorID() const {
    return 0x446B928F;
}

cIGZUnknown *cGZGraphicWin32COMDirector::CreatecGZGraphicSystemWin32() {
    return new cGZGraphicSystemWin32();
}

cIGZUnknown *cGZGraphicWin32COMDirector::CreatecGZCanvasW32() {
    return new cGZCanvasW32();
}
