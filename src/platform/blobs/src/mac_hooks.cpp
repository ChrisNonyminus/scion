#define HANDLE void*
#define DADiskRef void*
#define acl_t void*
#define vm_address_t uintptr_t
#define mach_vm_address_t vm_address_t
#define mach_vm_size_t size_t
#define CFMachPortRef void*


#include "glad/gl.h"
#include "mac_hooks.h"
#include <SDL2/SDL_opengl.h>

#ifdef TARGET_GAME_sc4
#define _CGDisplayPixelsWide_ADDR 0x00C74183
#define _CGDisplayPixelsHigh_ADDR 0x00C741D3
#define _CGDisplayBitsPerPixel_ADDR 0x00C74179
#define _CGDisplayIOServicePort_ADDR 0x00C73F1C
#define _CGDisplayBestModeForParameters_ADDR 0x00C73DC8
#define _SetThemeCursor_ADDR 0x00C73C06
#elif TARGET_GAME_sims2
#include "../The Sims 2 Bon Voyage~.x86_imp_addrs.txt"
#endif

#include <signal.h>
#include <sys/sysinfo.h>

#include <vector>

#include "Carbon.h"
#include "ld-mac.hh"

size_t CGDisplayPixelsWide(CGDirectDisplayID displayID) {
  SDL_DisplayMode mode;
  if (SDL_GetCurrentDisplayMode(0, &mode) < 0) {
    printf("ERROR! Called SDL_GetDisplayMode but got error: %s!\n",
           SDL_GetError());
    raise(SIGSEGV);
  }
  return mode.w;
}

size_t CGDisplayPixelsHigh(CGDirectDisplayID displayID) {
  SDL_DisplayMode mode;
  if (SDL_GetCurrentDisplayMode(0, &mode) < 0) {
    printf("ERROR! Called SDL_GetDisplayMode but got error: %s!\n",
           SDL_GetError());
    raise(SIGSEGV);
  }
  return mode.h;
}

size_t CGDisplayBitsPerPixel(CGDirectDisplayID displayID) {
  SDL_DisplayMode mode;
  if (SDL_GetCurrentDisplayMode(0, &mode) < 0) {
    printf("ERROR! Called SDL_GetDisplayMode but got error: %s!\n",
           SDL_GetError());
    raise(SIGSEGV);
  }
  return SDL_BITSPERPIXEL(mode.format);
}
io_object_t
    gDisplayServicePorts[8]; // assume the user won't have more than 8 displays
io_object_t CGDisplayIOServicePort(
    uint32_t displayID
) {
  return gDisplayServicePorts[displayID];
}

CFDictionaryRef CGDisplayBestModeForParameters(CGDirectDisplayID display,
                                               size_t bitsPerPixel,
                                               size_t width,
                                               size_t height,
                                               boolean_t *exactMatch) {
  uint32_t videoModeCount = SDL_GetNumDisplayModes(0);
  if (videoModeCount == 0) {
    return NULL;
  }
  SDL_DisplayMode *modes = (SDL_DisplayMode *) malloc(
      sizeof(SDL_DisplayMode) * videoModeCount);
  for (int i = 0; i < videoModeCount; i++) {
    SDL_GetDisplayMode(0, i, &modes[i]);
    //if (SDL_BITSPERPIXEL(modes[i].format) == bpp && modes[i].w == width &&
    //modes[i].h == height) {
    *exactMatch = 1;
    //}
  }
  free(modes);
  return NULL;
}

OSStatus
SetThemeCursor(ThemeCursor inCursor) {
  switch (inCursor) {
  case 0:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    break;
  case 1:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    break;
  case 2:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    break;
  case 3:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    break;
  case 4:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM));
    break;
  case 5:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR));
    break;
  case 6:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    break;
  case 7:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT));
    break;
  case 8:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    break;
  case 9:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    break;
  case 10:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    break;
  case 11:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    break;
  case 12:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    break;
  case 13:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
    break;
  case 14:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT));
    break;
  case 15:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE));
    break;
  case 16:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE));
    break;
  case 17:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE));
    break;
  case 18:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO));
    break;
  case 19:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS));
    break;
  case 20:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS));
    break;
  case 21:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS));
    break;
  case 22:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO));
    break;
  default:SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    break;
  }
  return 0;
}

// Threads.h

static pthread_mutex_t tlThreadMutex =  PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
OSErr ThreadBeginCritical() {
  pthread_mutex_lock(&tlThreadMutex);
  return 0;
}

OSErr ThreadEndCritical() {
  pthread_mutex_unlock(&tlThreadMutex);
  return 0;
}

OSErr GetCurrentProcess(ProcessSerialNumber *PSN) {
  PSN->lowLongOfPSN = getpid();
  return 0;
}

OSStatus GetProcessPID(const ProcessSerialNumber *psn, pid_t *pid) {
  *pid = psn->lowLongOfPSN;
  return 0;
}

OSErr GetCurrentThread(ThreadID* currentThreadID) {
  *currentThreadID = pthread_self();
  return 0;
}

CGDisplayErr CGGetActiveDisplayList(CGDisplayCount maxDisplays,
                                    CGDirectDisplayID * activeDspys,
                                    CGDisplayCount * dspyCnt) {
  if (/*maxDisplays < 1 || activeDspys == NULL ||*/ dspyCnt == NULL) {
    return -1;
  }
  *dspyCnt = 1;
  if (activeDspys != NULL) {
    activeDspys[0] = new _CGDirectDisplayID;
    (activeDspys[0])->displayIdx = 0;
  }
  return 0;
}

CGOpenGLDisplayMask CGDisplayIDToOpenGLDisplayMask(CGDirectDisplayID display) {
  return 1;
}

struct __CFString {
  void *isa;
  int unk;
  char *str;
  int unk2;
};

struct __CFURL {
  const char* str;
};

CFURLRef CFBundleCopyResourceURL(
    CFBundleRef bundle,
    CFStringRef resourceName,
    CFStringRef resourceType,
    CFStringRef subDirName) {
  char path[1024];
  memset(path, 0, 1024);
  strcpy(path, getenv("HOME"));
  strcat(path, "/.scion/MacRes/");
  if (subDirName != NULL) {
    strcat(path, subDirName->str);
    strcat(path, "/");
  }
  strcat(path, resourceName->str);
  __CFURL* ref = static_cast<__CFURL *>(malloc(sizeof(__CFURL)));
  ref->str = strdup(path);
  return ref;
}

Boolean CFURLGetFileSystemRepresentation(CFURLRef url, Boolean
resolveAgainstBase, UInt8 *buffer, CFIndex maxBufLen) {
  if (!url || !url->str) return false;
  strncpy(reinterpret_cast<char *>(buffer), url->str, maxBufLen);
  return true;
}

void noOp() {}

CFBundleRef CFBundleGetMainBundle() {
  return NULL;
}

void CFRelease(CFTypeRef ref) {
  free(const_cast<void*>(ref));
}

OSStatus MacPath2FSRef() {
  return -1; // stub
}

CGError CGGetDisplaysWithOpenGLDisplayMask(CGOpenGLDisplayMask mask, uint32_t
maxDisplays, CGDirectDisplayID* displays, uint32_t* matchingDisplayCount) {
  for (int i = 0; i < SDL_GetNumVideoDisplays(); i++) {
    displays[i] = new _CGDirectDisplayID;
    (displays[i])->displayIdx = i;
  }
  *matchingDisplayCount = SDL_GetNumVideoDisplays();
  return 0;
}

CGDirectDisplayID CGMainDisplayID(void) {
  static CGDirectDisplayID mainDisplay = new _CGDirectDisplayID;
  mainDisplay->displayIdx = 0;
  return mainDisplay;
}
extern OSErr 
DMGetGDeviceByDisplayID(
  DisplayIDType   displayID,
  GDHandle *      displayDevice,
  Boolean         failToMain) {
    return -1;
  }

void __ZN16CQuickTimePlayer7PlayAllEPbPFvvEb(void* this_ptr, bool* isDone,
                                             void(*)(), bool) {
  *isDone = true;
}

struct noOpvtbl {
  void (*noOp[2])();
};


void __ZN16CQuickTimePlayerC1EPKcbbNS_9ePosFlagsE(noOpvtbl **this_ptr) {
  *this_ptr = new noOpvtbl;
  ((noOpvtbl*)*this_ptr)->noOp[0] = noOp;
  ((noOpvtbl*)*this_ptr)->noOp[1] = noOp;
}
void __ZN16CQuickTimePlayer11SetMovieBoxEP4Rect() {}

// TODO
FNSubscriptionUPP
NewFNSubscriptionUPP(FNSubscriptionProcPtr userRoutine) {
  return userRoutine;
}
OSStatus  FNSubscribeByPath(const UInt8 *directoryPath, FNSubscriptionUPP
callback, void *refcon, OptionBits flags, FNSubscriptionRef *subscription) {
  return 0;
}

void HookMiscCarbonFunctions() {
  HOOK_FUNC(SetThemeCursor)
  HOOK_FUNC(ThreadBeginCritical)
  HOOK_FUNC(ThreadEndCritical)

  HOOK_FUNC(GetCurrentProcess)
  HOOK_FUNC(GetProcessPID);

  HOOK_FUNC(GetCurrentThread)

  HOOK_FUNC(NewFNSubscriptionUPP)
  HOOK_FUNC(FNSubscribeByPath)

  /*HOOK_FUNC(GetCurrentEventLoop)
  HOOK_FUNC(GetMainEventLoop)
  HOOK_FUNC(RunCurrentEventLoop)
  HOOK_FUNC(QuitEventLoop)
  HOOK_FUNC(InstallEventLoopTimer)
  HOOK_FUNC(GetCurrentEventTime)*/

  hook_function(gMachO->GetSymbolAddr("_MacPath2FSRef"), CAST_CPP_FUNCPTR
  (MacPath2FSRef));

  hook_function(gMachO->GetSymbolAddr
  ("__ZN16CQuickTimePlayerC1EPKcbbNS_9ePosFlagsE"), CAST_CPP_FUNCPTR
  (__ZN16CQuickTimePlayerC1EPKcbbNS_9ePosFlagsE));
  hook_function(gMachO->GetSymbolAddr
  ("__ZN16CQuickTimePlayer11SetMovieBoxEP4Rect"), CAST_CPP_FUNCPTR
  (__ZN16CQuickTimePlayer11SetMovieBoxEP4Rect));
  hook_function(gMachO->GetSymbolAddr(
      ("__ZN16CQuickTimePlayer7PlayAllEPbPFvvEb")), CAST_CPP_FUNCPTR
      (__ZN16CQuickTimePlayer7PlayAllEPbPFvvEb));

  /*uint8_t codeToDisableQuickTime[6] = {0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
  memcpy((void*)0x00C1F59E, codeToDisableQuickTime, 6);*/
}

void HookCoreFoundationFunctions() {
  HOOK_FUNC(CFBundleCopyResourceURL);
  HOOK_FUNC(CFURLGetFileSystemRepresentation);
  HOOK_FUNC(CFBundleGetMainBundle)
  HOOK_FUNC(CFRelease)
}
void HookCoreGraphicsFunctions() {
  HOOK_FUNC(CGDisplayPixelsWide)
  HOOK_FUNC(CGDisplayPixelsHigh)
  HOOK_FUNC(CGDisplayBitsPerPixel)
  //HOOK_FUNC(CGDisplayIOServicePort)
  HOOK_FUNC(CGDisplayBestModeForParameters)
  HOOK_FUNC(CGGetActiveDisplayList)
  HOOK_FUNC(CGDisplayIDToOpenGLDisplayMask)
  HOOK_FUNC(CGGetDisplaysWithOpenGLDisplayMask)
  HOOK_FUNC(CGMainDisplayID)
}