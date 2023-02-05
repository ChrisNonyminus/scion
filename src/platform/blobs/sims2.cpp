#include <stdint.h>

#include <vector>

#define MACHO_EXE_END 0x2320FA8

#define MACHO_SEGMENT(segname, vmaddr, vmsize, fileoff, filesize) \
static const uint32_t MACHO_ ## segname ## _ADDRESS = vmaddr; \
static const uint32_t MACHO_ ## segname ## _MEM_SIZE = vmsize; \
static const uint32_t MACHO_ ## segname ## _FILE_OFF = fileoff; \
static const uint32_t MACHO_ ## segname ## _FILE_SIZE = filesize;

MACHO_SEGMENT(TEXT, 0x1000, 0x01ce0000, 0x0, 0x01ce0000)
MACHO_SEGMENT(DATA, 0x01ce1000, 0x00623000, 0x01ce0000, 0x0030c000)
MACHO_SEGMENT(IMPORT, 0x02304000, 0x0001c000, 0x01fec000, 0x0001c000)
//MACHO_SEGMENT(OBJC, 0xC75000, 0x1000, 0xBF5000, 0x1000)

MACHO_SEGMENT(__mod__init_func, 0x01cf1358, 0x00001684, 0x01cf0358, 0x00001684)


typedef void (*StaticInitFunc)();
std::vector<StaticInitFunc> gInitFuncs;

#include "map_macho.h"
#include "hooks.h"

#include "cRZCmdLine.h"

#include <SDL2/SDL.h>

extern "C" int RealMain(); // the WinMain function in
// the game's binary

namespace ASL {
extern char mAppCmdLine[];
extern bool mNeedsUnicodeTextInput;
}

// static initializers I have to call from here
void StaticInit() {
  /*for (auto& func : gInitFuncs) {
    func();
  }*/
}

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("ERROR! Called SDL_Init but got error: %s\n", SDL_GetError());
    exit(-1);
  }

  MapMacho("The Sims 2 Bon Voyage~.x86");

  StaticInit();

  // build the command line
  ASL::mAppCmdLine[0] = ' ';
  for (int i = 1; i < argc; i++) {
    strcat(ASL::mAppCmdLine, argv[i]);
    strcat(ASL::mAppCmdLine, " ");
  }

  ASL::mNeedsUnicodeTextInput = true;
  

  return RealMain();
}