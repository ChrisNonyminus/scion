//
// Created by chris on 1/17/2023.
//

#ifndef SCION_SC4STATICS_H
#define SCION_SC4STATICS_H

#include "cRZCmdLine.h"
#include "cRZString.h"

int SC4AppStart(cRZCmdLine *cmdLine, bool a2);
int SC4GamePause(bool a1, bool a2, bool a3, bool a4, bool a5);
void SC4OSMessageBoxAlert(const cRZString &msg);
void SC4AdjustPathSeparatorsForPlatform(const cRZString &in,
                                        cRZString &out);
extern bool gbAppResourcesInitializedOK;
extern uint32_t gbAppResourceErrorLanguage;
#endif //SCION_SC4STATICS_H
