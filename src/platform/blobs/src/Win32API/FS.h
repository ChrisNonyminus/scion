#ifndef BLOBS_SRC_WIN32API_FS_H
#define BLOBS_SRC_WIN32API_FS_H

#include "Handle.h"
#include "WinUtils.h"

#include <stdio.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <dirent.h>

#include <boost/filesystem.hpp>

struct FindResult {
  DIR *dir;
  dirent *it;
  const char *szWildcard;
  boost::filesystem::path fullpath;
};
extern "C" {
uint32_t GetFileAttributesA(/*in*/ const char *szFilePath);
HANDLE CreateFileA(
    /*in*/ const char *lpFileName,
    /*in*/ uint32_t dwDesiredAccess,
    /*in*/ uint32_t dwShareMode,
    /*in, optional*/ void *lpSecurityAttributes,
    /*in*/ uint32_t dwCreationDisposition,
    /*in*/ uint32_t dwFlagsAndAttributes,
    /*in, optional*/ void *hTemplateFile
);
bool ReadFile(
    /*in*/ HANDLE hFile,
    /*out*/ void *lpBuffer,
    /*in*/ uint32_t nNumberOfBytesToRead,
    /*out, optional*/ uint32_t *lpNumberOfBytesRead,
    /*in, out, optional*/ void *lpOverlapped
);
bool WriteFile(
    /*in*/ HANDLE hFile,
    /*in*/ const void *lpBuffer,
    /*in*/ uint32_t nNumberOfBytesToWrite,
    /*out, optional*/ uint32_t *lpNumberOfBytesWritten,
    /*in, out, optional*/ void *lpOverlapped
);
DWORD SetFilePointer(
    /*in*/ HANDLE hFile,
    /*in*/ LONG lDistanceToMove,
    /*in, out, optional*/ LONG* lpDistanceToMoveHigh,
    /*in*/ DWORD dwMoveMethod
    );

uint32_t GetFileSize(
    /*in*/ HANDLE hFile,
    /*out, optional*/ uint32_t *lpFileSizeHigh
);
bool CreateDirectoryA(
    /*in*/ const char *lpPathName,
    /*in, optional*/ void *lpSecurityAttributes
);
bool RemoveDirectoryA(
    /*in*/ const char *lpPathName
);
bool DeleteFileA(
    /*in*/ const char *lpFileName
);
bool CreateDirectoryW(
    /*in*/ const wchar_t *lpPathName,
    /*in, optional*/ void *lpSecurityAttributes
);
bool RemoveDirectoryW(
    /*in*/ const wchar_t *lpPathName
);
struct WIN32_FIND_DATA {
  uint32_t dwFileAttributes;
  uint32_t ftCreationTime;
  uint32_t ftLastAccessTime;
  uint32_t ftLastWriteTime;
  uint32_t nFileSizeHigh;
  uint32_t nFileSizeLow;
  uint32_t dwReserved0;
  uint32_t dwReserved1;
  char cFileName[260];
};
struct WIN32_FIND_DATAW {
  uint32_t dwFileAttributes;
  uint32_t ftCreationTime;
  uint32_t ftLastAccessTime;
  uint32_t ftLastWriteTime;
  uint32_t nFileSizeHigh;
  uint32_t nFileSizeLow;
  uint32_t dwReserved0;
  uint32_t dwReserved1;
  wchar_t cFileName[260];
};
HANDLE FindFirstFileA(
    /*in*/ const char *lpFileName, /* NOTE: The filename can include
                                    * wildcard characters, such as an
                                    * asterisk or a question mark.
                                    */
    /*out*/ WIN32_FIND_DATA *lpFindFileData // pointer to information about a
    // found file/directory
);
HANDLE FindFirstFileW(
    /*in*/ const wchar_t *lpFileName, /* NOTE: The filename can include
                                      * wildcard characters, such as an
                                      * asterisk or a question mark.
                                      */
    /*out*/ WIN32_FIND_DATAW *lpFindFileData // pointer to information about a
    // found file/directory
);
bool FindNextFileA(
    /*in*/ HANDLE hFindFile,
    /*out*/ WIN32_FIND_DATA *lpFindFileData
);
bool FindNextFileW(
    /*in*/ HANDLE hFindFile,
    /*out*/ WIN32_FIND_DATAW *lpFindFileData
);
bool FindClose(
    /*in*/ HANDLE hFindFile
);
uint32_t SHGetFolderPathA(
    /*in*/ void *hwnd, // reserved
    /*in*/ int csidl, // identifies the folder whose path is to be retrieved
    /*in*/ void *hToken, // an access token which can be used to represent a
    // particular user (usually null)
    /*in*/ int dwFlags, // ignored
    /*out*/ char *pszPath // the output folder path
);
uint32_t SHGetFolderPathW(
    /*in*/ void *hwnd, // reserved
    /*in*/ int csidl, // identifies the folder whose path is to be retrieved
    /*in*/ void *hToken, // an access token which can be used to represent a
    // particular user (usually null)
    /*in*/ int dwFlags, // ignored
    /*out*/ wchar_t *pszPath // the output folder path
);

/*
 * Sets the physical file size for the specified file to the current position
 * of the file pointer.
 */
bool SetEndOfFile(
    /*in*/ HANDLE hFile
);


};
extern "C"
void __Z9SplitpathPKcPcS1_S1_S1_(char* path, char* drive, char* dir, char*
fname, char* ext);

#endif //BLOBS_SRC_WIN32API_FS_H
