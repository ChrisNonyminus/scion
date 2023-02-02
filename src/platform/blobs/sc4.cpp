#include <stdint.h>

#include <vector>

#define MACHO_EXE_END 0xC7E294

#define MACHO_SEGMENT(segname, vmaddr, vmsize, fileoff, filesize) \
static const uint32_t MACHO_ ## segname ## _ADDRESS = vmaddr; \
static const uint32_t MACHO_ ## segname ## _MEM_SIZE = vmsize; \
static const uint32_t MACHO_ ## segname ## _FILE_OFF = fileoff; \
static const uint32_t MACHO_ ## segname ## _FILE_SIZE = filesize;

MACHO_SEGMENT(TEXT, 0x1000, 0xAB6000, 0x0, 0xAB6000)
MACHO_SEGMENT(DATA, 0xAB7000, 0x1BB000, 0xAB6000, 0x13C000)
MACHO_SEGMENT(IMPORT, 0xC72000, 0x3000, 0xBF2000, 0x3000)
MACHO_SEGMENT(OBJC, 0xC75000, 0x1000, 0xBF5000, 0x1000)

MACHO_SEGMENT(__mod__init_func, 0x00BF25AC, 0x00000788, 0x00bf15ac, 0x00000788)

extern "C" void __GLOBAL__I__ZN16cGZCanvasMessage10mAllocatorE();
extern "C" void __GLOBAL__I__ZN7nSGLDX717gDriverInfoPrefixE();
extern "C" void __GLOBAL__I__Z31GZCOM_CreatecRZMessage2Standardv();
extern "C" void __GLOBAL__I__ZN16cGZGraphicSystem21mbDesiredWindowedModeE();

typedef void (*StaticInitFunc)();
std::vector<StaticInitFunc> gInitFuncs;

#include "map_macho.h"
#include "hooks.h"

#include "cRZCmdLine.h"

#include <SDL2/SDL.h>

#include "cGZGraphicSystem.h"

#include "cIGZExceptionBase.h"

int SC4AppStart(cRZCmdLine const &, bool);

// static initializers I have to call from here
void StaticInit() {
  /*__GLOBAL__I__ZN16cGZCanvasMessage10mAllocatorE();
  __GLOBAL__I__ZN7nSGLDX717gDriverInfoPrefixE();
  __GLOBAL__I__Z31GZCOM_CreatecRZMessage2Standardv();
  __GLOBAL__I__ZN16cGZGraphicSystem21mbDesiredWindowedModeE();*/
  for (auto& func : gInitFuncs) {
    func();
  }
}

extern int kPackedFileHeaderFileIdentifier;

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("ERROR! Called SDL_Init but got error: %s\n", SDL_GetError());
    exit(-1);
  }

  MapMacho("SimCity 4 Rush Hour~.x86");

  StaticInit();

  // concatate argv into one string
  char *cmdline = (char *) malloc(1);
  cmdline[0] = 0;
  for (int i = 1; i < argc; i++) {
    cmdline = (char *) realloc(cmdline, strlen(cmdline) + strlen(argv[i]) + 2);
    strcat(cmdline, argv[i]);
    strcat(cmdline, " ");
  }
  // end cmdline with -w
  cmdline = (char *) realloc(cmdline, strlen(cmdline) + 2);
  strcat(cmdline, " -w ");

  cRZCmdLine rzcmdline(cmdline);

    SC4AppStart(rzcmdline, false);

  return 0;
}