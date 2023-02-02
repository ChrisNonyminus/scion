#ifndef BLOBS_SRC_RZWINAPI_H
#define BLOBS_SRC_RZWINAPI_H

#include "hooks.h"

#ifdef TARGET_GAME_sims2
DECL_EXTERN_C_FUNC(_ZN12cRZDirAPIW32C2Ev, 0x0050CCF0)
DECL_EXTERN_C_FUNC(_ZN13cRZFileAPIW32C2Ev, 0x005138C4)
DECL_EXTERN_C_FUNC(_ZN36ShellExecuteFunctionEntryPointFinderC2Ev, 0x0053F474)

#include "../The Sims 2 Bon Voyage~.x86_winapi_addrs.txt"

#endif

// stubbed class
struct cRZDirAPIW32 {
  void *MoveFileExW, *FindFirstFileW, *FindNextFileW, *CreateDirectoryW,
      *RemoveDirectoryW, *GetWindowsDirectoryW, *SHGetFolderPathW;
  bool field_1C;
  cRZDirAPIW32();
};

// not stubbed class
/*
 *  cRZFileAPIW32
 *
 *  Holds the following structure:
 *  -Pointer to implementation of CreateFileW
 *  -Pointer to implementation of CopyFileW
 *  -Pointer to implementation of MoveFileW
 *  -Pointer to implementation of FindFirstFileW
 *  -Pointer to implementation of DeleteFileW
 *  -Pointer to implementation of GetFileAttributesW
 *  -Pointer to implementation of GetLongPathNameA
 *  -Pointer to implementation of GetLongPathNameW
 *  -Pointer to implementation of GetTempPathW
 *  -Pointer to implementation of GetDiskFreeSpaceExA
 *  -Pointer to implementation of GetDiskFreeSpaceExW
 *  -A boolean that is true if the API is implemented
 */
typedef uint32_t
(*CreateFileW_t)(const wchar_t *lpFileName, uint32_t dwDesiredAccess,
                 uint32_t dwShareMode, void *lpSecurityAttributes,
                 uint32_t dwCreationDisposition, uint32_t dwFlagsAndAttributes,
                 void *hTemplateFile);
typedef uint32_t
(*CopyFileW_t)(const wchar_t *lpExistingFileName, const wchar_t *lpNewFileName,
               bool bFailIfExists);
typedef uint32_t
(*MoveFileW_t)(const wchar_t *lpExistingFileName, const wchar_t *lpNewFileName);
typedef uint32_t
(*FindFirstFileW_t)(const wchar_t *lpFileName, void *lpFindFileData);
typedef uint32_t (*DeleteFileW_t)(const wchar_t *lpFileName);
typedef uint32_t (*GetFileAttributesW_t)(const wchar_t *lpFileName);
typedef uint32_t
(*GetLongPathNameA_t)(const char *lpszShortPath, char *lpszLongPath,
                      uint32_t cchBuffer);
typedef uint32_t
(*GetLongPathNameW_t)(const wchar_t *lpszShortPath, wchar_t *lpszLongPath,
                      uint32_t cchBuffer);
typedef uint32_t (*GetTempPathW_t)(uint32_t nBufferLength, wchar_t *lpBuffer);
typedef uint32_t(*GetDiskFreeSpaceExA_t)
    (const char *lpDirectoryName, uint64_t *lpFreeBytesAvailableToCaller,
     uint64_t *lpTotalNumberOfBytes, uint64_t *lpTotalNumberOfFreeBytes);
typedef uint32_t(*GetDiskFreeSpaceExW_t)
    (const wchar_t *lpDirectoryName, uint64_t *lpFreeBytesAvailableToCaller,
     uint64_t *lpTotalNumberOfBytes, uint64_t *lpTotalNumberOfFreeBytes);

struct cRZFileAPIW32 {
  CreateFileW_t CreateFileW;
  CopyFileW_t CopyFileW;
  MoveFileW_t MoveFileW;
  FindFirstFileW_t FindFirstFileW;
  DeleteFileW_t DeleteFileW;
  GetFileAttributesW_t GetFileAttributesW;
  GetLongPathNameA_t GetLongPathNameA;
  GetLongPathNameW_t GetLongPathNameW;
  GetTempPathW_t GetTempPathW;
  GetDiskFreeSpaceExA_t GetDiskFreeSpaceExA;
  GetDiskFreeSpaceExW_t GetDiskFreeSpaceExW;
  bool field_2C;
  cRZFileAPIW32();
};

// Shims the devs made from WinAPI to Mac. We'll reimplement them, but shim
// for linux/SDL instead.


void DoRZWinAPIHooks();

#endif //BLOBS_SRC_RZWINAPI_H
