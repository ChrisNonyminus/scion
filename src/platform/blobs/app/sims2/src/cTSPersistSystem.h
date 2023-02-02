#ifndef BLOBS_APP_SIMS2_SRC_CTSPERSISTSYSTEM_H
#define BLOBS_APP_SIMS2_SRC_CTSPERSISTSYSTEM_H

#include "cITSPersistSystem.h"
#include "cRZThread.h"

/*TODO*/
/* cTSPersistSystem is temporarily a struct with its base classes defined as
 * separate members
 * until I implement those base classes
 * REASON: this class has a big-ass vtable, er... vtables, plural.
 * */

struct cTSPersistSystemVtable1 {

};
struct cTSPersistSystemVtable2 {

};
struct cTSPersistSystemVtable3 {

};
struct cTSPersistSystem {
  cTSPersistSystemVtable1* __vftable;
  cTSPersistSystemVtable2* __vftable_4;
  cTSPersistSystemVtable3* __vftable_8;
  uint8_t gapC[0x1A0 - 0xC];
  cIGZThread* thr;
  static uint32_t ForceOpenResources(void* userData);

  /*virtual*/ bool OpenResourcesInBackground();
};

#endif //BLOBS_APP_SIMS2_SRC_CTSPERSISTSYSTEM_H
