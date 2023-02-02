#include "cTSPersistSystem.h"
bool cTSPersistSystem::OpenResourcesInBackground() {
  if (!thr) {
    thr = new cRZThread();
    thr->SetThreadName("cTSPersistSystem_OpenResourcesInBackground");
    thr->SetPriority(2);

    /*ORIGINAL BEHAVIOR: In the game binary, this function just starts the
     * thread without waiting for it, and returns the return value of Begin().
     * I'm gonna see what happens when it waits instead (expected outcome: It
     * waits for all resources to be opened).
     **/
    thr->Begin(cTSPersistSystem::ForceOpenResources, this);
    if (thr->IsValid()) {
      thr->Join(-1);
      return true;
    }
    return false;
  }

  bool result = thr->IsValid();
  if (!result) {
    thr->Begin(cTSPersistSystem::ForceOpenResources, this);
    thr->Join(-1);
    return true;
  }
  thr->Join(-1);
  return true;

}
