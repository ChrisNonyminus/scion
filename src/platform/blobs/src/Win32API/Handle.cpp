
#include <dlfcn.h>
#include "Handle.h"
#include "FS.h"
#include "GDI.h"
#include "Windowing.h"

#include <signal.h>

bool CloseHandle(HANDLE hToClose) {
  if (hToClose->MAGIC != 'HNDL') {
    printf("ERROR! Handle not valid struct.\n");
    signal(SIGSEGV, SIG_DFL);
  }

  switch (hToClose->type) {
  case Handle_FindFile: {
    FindClose(hToClose);
    break;
  }
  case Handle_FileHandle: {
    if (hToClose->ptr != NULL)
      fclose((FILE *) hToClose->ptr);
    break;
  }
  case Handle_Brush: {
    if (hToClose->ptr != NULL)
      delete (HBRUSH_SDL *) hToClose->ptr;
    break;
  }
  case Handle_Instance: {
    if (hToClose->ptr != NULL)
      dlclose(hToClose->ptr);
    break;
  }
  case Handle_Window: {
    if (hToClose != NULL) {
      SDL_DestroyWindow((SDL_Window *) hToClose);
    }
    break;
  }
  default: return false;
  }
  delete hToClose;
  return true;
}

HANDLE GetModuleHandleA(const char *modName) {
  void *module = dlopen(modName, RTLD_LAZY);
  return HANDLE_Create(Handle_Instance, module);
}
HANDLE HANDLE_Create(HandleType type, void *ptr) {
  Handle *hndl = new Handle;
  hndl->ptr = ptr;
  hndl->type = type;
  hndl->MAGIC = 'HNDL';
  return hndl;

}
