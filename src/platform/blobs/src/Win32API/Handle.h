#ifndef BLOBS_SRC_WIN32API_HANDLE_H
#define BLOBS_SRC_WIN32API_HANDLE_H

#include "GZTypes.h"

enum HandleType {
  Handle_FileHandle,
  Handle_FindFile,
  Handle_Brush,
  Handle_Window,
  Handle_Instance
};

struct Handle {
  uint32_t MAGIC;
  HandleType type;
  void *ptr;
};

typedef Handle *HANDLE;

bool CloseHandle(HANDLE hToClose);

HANDLE HANDLE_Create(HandleType type, void *ptr);

HANDLE GetModuleHandleA(const char *modName);

#endif //BLOBS_SRC_WIN32API_HANDLE_H
