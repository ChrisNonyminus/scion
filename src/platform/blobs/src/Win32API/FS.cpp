/*
 * TODO:
 * Known bugs:
 * String encoding seems to be fucked. The game randomly makes files with
 * corrupted filenames, and folders with corrupted names too. dunno why.
 * */

#include "FS.h"

#include <fnmatch.h>

//#include <codecvt>

const char *DOSPathToUnixPath(const char *szDosPath) {
  static char szUnixPath[1024];
  strcpy(szUnixPath, szDosPath);
  for (int i = 0; i < strlen(szUnixPath); i++) {
    if (szUnixPath[i] == '\\') {
      szUnixPath[i] = '/';
    }
  }
  szUnixPath[strlen(szUnixPath)] = '\0';
  if (strstr(szUnixPath,"C:")) {
    return strdup(szUnixPath + 2);
  }
  return strdup(szUnixPath);
}

std::string fixPath(std::string str)
{
  std::string tmp = "";
  for(size_t i = 0; i < str.length(); i++)
  {
    if(str[i] != ' ')
      tmp += str[i];
    else
      tmp += "\\ ";
  }
  return tmp;
}

enum FileAttribute {
  FILE_ATTRIBUTE_ARCHIVE = 32,
  FILE_ATTRIBUTE_COMPRESSED = 2048,
  FILE_ATTRIBUTE_DEVICE = 64,
  FILE_ATTRIBUTE_DIRECTORY = 16,
  FILE_ATTRIBUTE_ENCRYPTED = 16384,
  FILE_ATTRIBUTE_HIDDEN = 2,
  FILE_ATTRIBUTE_NORMAL = 128, FILE_ATTRIBUTE_NOT_CONTENT_INDEXED = 8192,
  FILE_ATTRIBUTE_NO_SCRUB_DATA = 131072, FILE_ATTRIBUTE_OFFLINE = 4096,
  FILE_ATTRIBUTE_READONLY = 1, FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS = 4194304,
  FILE_ATTRIBUTE_RECALL_ON_OPEN = 262144, FILE_ATTRIBUTE_REPARSE_POINT = 1024,
  FILE_ATTRIBUTE_SPARSE_FILE = 512, FILE_ATTRIBUTE_SYSTEM = 4,
  FILE_ATTRIBUTE_TEMPORARY = 256, FILE_ATTRIBUTE_VIRTUAL = 65536,
  FILE_ATTRIBUTE_PINNED = 524288, FILE_ATTRIBUTE_UNPINNED = 1048576
};

enum FileAccess {
  GENERIC_READ = 0x80000000,
  GENERIC_WRITE = 0x40000000,
  GENERIC_EXECUTE = 0x20000000,
  GENERIC_ALL = 0x10000000
};

uint32_t GetFileAttributesA(const char *szFilePath) {
  boost::filesystem::path p(szFilePath);
  if (!boost::filesystem::exists(p)) {
    return -1;
  }
  uint32_t ret = 0;
  if (boost::filesystem::is_regular_file(p)) {
    ret |= FILE_ATTRIBUTE_NORMAL;
  }
  if (boost::filesystem::is_directory(p)) {
    ret |= FILE_ATTRIBUTE_DIRECTORY;
  }
  if (boost::filesystem::is_symlink(p)) {
    ret |= FILE_ATTRIBUTE_REPARSE_POINT;
  }
  /*if (p.root_name() == "/tmp") {
    ret |= FILE_ATTRIBUTE_TEMPORARY;
  }
  if (p.root_name() == "/dev") {
    ret |= FILE_ATTRIBUTE_DEVICE;
  }*/
  return ret;
}

void mkdirpwrapper(const std::string & p) {
  if (!boost::filesystem::exists(p)) {

      char mkdircmd[1024];
      strcpy(mkdircmd, "mkdir -p ");
      strcat(mkdircmd, fixPath(p.substr(0, p
          .find_last_of("/"))).c_str());



      system(mkdircmd);
  }
}

HANDLE CreateFileA(const char *lpFileName,
                   uint32_t dwDesiredAccess,
                   uint32_t dwShareMode,
                   void *lpSecurityAttributes,
                   uint32_t dwCreationDisposition,
                   uint32_t dwFlagsAndAttributes,
                   void *hTemplateFile) {
  std::string szRealFileName = DOSPathToUnixPath(lpFileName);
  /*std::transform(szRealFileName.begin(), szRealFileName.end(), szRealFileName.begin(),
                 tolower);*/
  std::string p(szRealFileName);
  if (!boost::filesystem::exists(p)) {
    if (dwDesiredAccess & GENERIC_WRITE) {

      char mkdircmd[1024];
      strcpy(mkdircmd, "mkdir -p ");
      strcat(mkdircmd, fixPath(p.substr(0, p
          .find_last_of("/"))).c_str());



      system(mkdircmd);
    }
  }
  printf("[DEBUG, Win32API::FS] Creating (or opening?) a file at path \"%s\""
         ".\n", szRealFileName.c_str());
  // TODO/HACK: replace "TSBin/TSData" in szRealFileName with just "TSData"
  size_t pos = szRealFileName.find("TSBin/TSData");
  if (pos != std::string::npos) {
    szRealFileName = szRealFileName.substr(0, pos) + "TSData" +
        (szRealFileName.size() > pos + 13 ? szRealFileName
            .substr(pos + 13) : "/");
  }
  char *szMode = NULL;
  if (dwDesiredAccess == GENERIC_READ) {
    szMode = "rb";
  } else if (dwDesiredAccess == GENERIC_WRITE) {
    szMode = "wb";
  } else if (dwDesiredAccess == (GENERIC_READ | GENERIC_WRITE)) {
    szMode = "rb+";
  }
  if (szMode == NULL) {
    printf("[ERROR, Win32API::FS] Failed to determine mode for opening file. "
           "dwDesiredAccess was %d\n", dwDesiredAccess);
    return NULL;
  }
  FILE *fp = fopen(szRealFileName.c_str(), szMode);
  HANDLE h = HANDLE_Create(Handle_FileHandle, fp);
  return h;
}
bool ReadFile(HANDLE hFile,
              void *lpBuffer,
              uint32_t nNumberOfBytesToRead,
              uint32_t *lpNumberOfBytesRead,
              void *lpOverlapped) {
  *lpNumberOfBytesRead = 0;
  if (hFile == NULL) {
    sLastError = 5;
    return false;
  }
  FILE *fp = (FILE *) hFile->ptr;
  if (fp == NULL) {
    sLastError = 5;
    return false;
  }
  char filename[PATH_MAX];
  char proclnk[1024];
  sprintf(proclnk, "/proc/self/fd/%d", fileno(fp));
  size_t len = readlink(proclnk, filename, PATH_MAX);
  if (len > 0) {
    filename[len] = '\0';
    printf("[Win32API::FS, DEBUG] Reading from file \"%s\"...\n", filename);
  }
  DWORD maxsize = GetFileSize(hFile, NULL);
  uint32_t remaining = nNumberOfBytesToRead;
  if (remaining > maxsize) {
    remaining = maxsize;
  }
  uint8_t* buf = static_cast<uint8_t *>(lpBuffer);
  while (remaining > 0) {
    if (remaining > 8192) {
      *lpNumberOfBytesRead += fread(buf, 1, 8192, fp);
      remaining -= 8192;
      buf += 8192;
    } else {
      *lpNumberOfBytesRead += fread(buf, 1, remaining, fp);
      buf += remaining;
      remaining = 0;
    }
  }
  return true;
}
bool WriteFile(HANDLE hFile,
               const void *lpBuffer,
               uint32_t nNumberOfBytesToWrite,
               uint32_t *lpNumberOfBytesWritten,
               void *lpOverlapped) {
  *lpNumberOfBytesWritten = 0;
  if (hFile == NULL) {
    return false;
  }
  FILE *fp = (FILE *) hFile->ptr;
  if (fp == NULL) {
    return false;
  }
  char filename[PATH_MAX];
  char proclnk[1024];
  sprintf(proclnk, "/proc/self/fd/%d", fileno(fp));
  size_t len = readlink(proclnk, filename, PATH_MAX);
  if (len > 0) {
    filename[len] = '\0';
    printf("[Win32API::FS, DEBUG] Writing to file \"%s\"...\n", filename);
  }
  uint32_t remaining = nNumberOfBytesToWrite;
  const uint8_t* buf = static_cast<const uint8_t *>(lpBuffer);
  while (remaining > 0) {
    if (remaining > 8192) {
      *lpNumberOfBytesWritten += fwrite(buf, 1, 8192, fp);
      remaining -= 8192;
      buf += 8192;
    } else {
      *lpNumberOfBytesWritten += fwrite(buf, 1, remaining, fp);
      buf += remaining;
      remaining = 0;
    }
  }
  return true;

}
uint32_t GetFileSize(
    /*in*/ HANDLE hFile,
    /*out, optional*/ uint32_t *lpFileSizeHigh
) {
  if (hFile == NULL) {
    return -1;
  }
  FILE *fp = (FILE *) hFile->ptr;
  if (fp == NULL) {
    return -1;
  }
  size_t curpos = ftell(fp);
  fseek(fp, 0, SEEK_END);
  uint32_t ret = ftell(fp);
  fseek(fp, curpos, SEEK_SET);
  return ret;
}
bool CreateDirectoryA(const char *lpPathName, void *lpSecurityAttributes) {
  std::string p(lpPathName);
  /*std::transform(p.begin(), p.end(), p.begin(),
                 tolower);*/
  if (boost::filesystem::exists(p)) {
    return false;
  }
  mkdirpwrapper(p);
  return boost::filesystem::exists(p);
}
bool RemoveDirectoryA(const char *lpPathName) {
  std::string p(lpPathName);
  /*std::transform(p.begin(), p.end(), p.begin(),
                 tolower);*/
  if (!boost::filesystem::exists(p)) {
    return false;
  }
  boost::filesystem::remove_all(p);
  return !boost::filesystem::exists(p);
}
bool DeleteFileA(const char *lpFileName) {
  std::string p(lpFileName);
  /*std::transform(p.begin(), p.end(), p.begin(),
                 tolower);*/
  if (!boost::filesystem::exists(p)) {
    return false;
  }
  boost::filesystem::remove(p);
  return !boost::filesystem::exists(p);
}
bool CreateDirectoryW(const wchar_t *lpPathName, void *lpSecurityAttributes) {
  char szPathName[1024];
  wcstombs(szPathName, lpPathName, wcslen(lpPathName) + 1);
  szPathName[wcslen(lpPathName)] = '\0';
  return CreateDirectoryA(szPathName, lpSecurityAttributes);
}
bool RemoveDirectoryW(const wchar_t *lpPathName) {
  char szPathName[1024];
  wcstombs(szPathName, lpPathName, wcslen(lpPathName) + 1);
  szPathName[wcslen(lpPathName)] = '\0';
  return RemoveDirectoryA(szPathName);
}
bool FilenameMatchesWildcard(const char *szFilename, const char *szWildcard) {
  return fnmatch(szWildcard, szFilename, 0) == 0;
}
HANDLE FindFirstFileRecursive(dirent* it, DIR* dir, std::string wildcard,
                              std::string parent_path);
HANDLE FindFirstFileA(
    /*in*/ const char *lpFileName, /* NOTE: The filename can include
                                    * wildcard characters, such as an
                                    * asterisk or a question mark.
                                    */
    /*out*/ WIN32_FIND_DATA *lpFindFileData // pointer to information about a
    // found file/directory
) {
  // take into account the wildcard characters (*.package should, for example, resolve to any .package file)
  bool bSearchAllFiles = false;
  lpFileName = DOSPathToUnixPath(lpFileName);
  if (*(uint8_t *) lpFileName > 0x7F || lpFileName[0] == '\0') {
    // not a valid filename? memory might be corrupted?
    return NULL;
  }
  std::string szRealFileName;
  if (
      lpFileName[strlen(lpFileName) - 2] == '/') {
    bSearchAllFiles = true;
    szRealFileName = lpFileName;
    szRealFileName = szRealFileName.substr(0, szRealFileName.length() - 2);
  } else if (
      lpFileName[strlen(lpFileName) - 3] == '/') {
    bSearchAllFiles = true;
    szRealFileName = lpFileName;
    szRealFileName = szRealFileName.substr(0, szRealFileName.length() - 3);
  } else {
    szRealFileName = lpFileName;
  }
  // TODO/HACK: replace "TSBin/TSData" in szRealFileName with just "TSData"
  size_t pos = szRealFileName.find("TSBin/TSData");
  if (pos != std::string::npos) {
    szRealFileName = szRealFileName.substr(0, pos) + "TSData" +
        (szRealFileName.size() > pos + 13 ? szRealFileName
            .substr(pos + 13) : "/");
  }
  /*std::transform(szRealFileName.begin(), szRealFileName.end(), szRealFileName.begin(), tolower);*/
  std::string parent_path = (szRealFileName.substr(0, szRealFileName
      .find_last_of("/")));
  if (!boost::filesystem::exists(szRealFileName) && szRealFileName.find("*"
  ) == std::string::npos) {
    // get outta here, something's wrong with the filename
    FindResult *pResult = new FindResult;
    pResult->it = NULL;
    pResult->dir = NULL;
    pResult->szWildcard = NULL;
    HANDLE handle = HANDLE_Create(Handle_FindFile, pResult);
    return handle;
  }
  if (!boost::filesystem::exists(bSearchAllFiles ? szRealFileName : parent_path)) {
    printf("[Win32API::FS] Failed to resolve file search query, folder does "
           "not exist. \"%s\".\n",
           bSearchAllFiles ? szRealFileName.c_str() : parent_path.c_str());
    return NULL;
  }
  DIR *dir = opendir(bSearchAllFiles ? szRealFileName.c_str()
                                     : parent_path.c_str());
  std::string wildcard = lpFileName;
  /*std::transform(wildcard.begin(), wildcard.end(), wildcard.begin(),
                 tolower);*/
  dirent *it = readdir(dir);
  for (; it != NULL; it = readdir(dir)) {
    if (strcmp(it->d_name, ".") == 0 || strcmp(it->d_name,
                                                        "..") == 0) {
      continue;
    }
    char fullpath[1024];
    strcpy(fullpath, (bSearchAllFiles ? szRealFileName.c_str()
                                      : parent_path.c_str()));
    strcat(fullpath, "/");
    strcat(fullpath, it->d_name);
    /*if (it->d_type == DT_DIR) {
      HANDLE maybe = FindFirstFileRecursive(it, dir, wildcard, (bSearchAllFiles ? szRealFileName.c_str()
                                                                                : parent_path.c_str()));
      if (maybe != NULL) {
        return maybe;
      }
    }*/
    if (bSearchAllFiles || FilenameMatchesWildcard(fullpath,
                                                   wildcard.c_str())) {
      /*if (boost::filesystem::is_directory(boost::filesystem::path(fullpath)))
        continue;*/
      lpFindFileData->dwFileAttributes =
          GetFileAttributesA(fullpath);
      lpFindFileData->nFileSizeHigh = 0;
      lpFindFileData->nFileSizeLow =
          boost::filesystem::is_directory(fullpath) ?
          0 : boost::filesystem::file_size(fullpath);
      strcpy(lpFindFileData->cFileName, it->d_name);
      FindResult *pResult = new FindResult;
      pResult->it = readdir(dir);
      pResult->dir = dir;
      pResult->szWildcard = bSearchAllFiles ? strdup(wildcard.c_str()) :
          szRealFileName
          .c_str();
      pResult->fullpath = bSearchAllFiles ? szRealFileName
                                          : parent_path;
      printf("FindFirstFileA: %s\n", lpFindFileData->cFileName);
      HANDLE handle = HANDLE_Create(Handle_FindFile, pResult);
      return handle;
    }
  }
  printf("[Win32API::FS] Failed to resolve file search query \"%s\".\n",
         lpFileName);
  return NULL;
}
HANDLE FindFirstFileRecursive(dirent* it, DIR* dir, std::string wildcard,
                              std::string parent_path) {
  char fullpath[1024];
  strcpy(fullpath, parent_path.c_str());
  strcat(fullpath, "/");
  strcat(fullpath, it->d_name);
  if (boost::filesystem::is_directory(boost::filesystem::path(fullpath))) {
    DIR* dir2 = opendir(fullpath);
    dirent* it2 = readdir(dir2);

    for (; it2 != NULL; it2 = readdir(dir2)) {
      if (strcmp(it2->d_name, ".") == 0 || strcmp(it2->d_name,
                                                          "..") == 0) {
        continue;
      }
      char fullpath2[1024];
      strcpy(fullpath2, fullpath);
      strcat(fullpath2, "/");
      strcat(fullpath2, it2->d_name);
      if (boost::filesystem::is_directory(boost::filesystem::path(fullpath2))) {
        HANDLE maybe = FindFirstFileRecursive(it2, dir2, wildcard, fullpath);
        if (maybe != NULL) {
          return maybe;
        }
      }
      if (FilenameMatchesWildcard(fullpath2, wildcard.c_str())) {
        WIN32_FIND_DATA *lpFindFileData = new WIN32_FIND_DATA;
        lpFindFileData->dwFileAttributes =
            GetFileAttributesA(fullpath2);
        lpFindFileData->nFileSizeHigh = 0;
        lpFindFileData->nFileSizeLow =
            boost::filesystem::is_directory(fullpath2) ?
            0 : boost::filesystem::file_size(fullpath2);
        strcpy(lpFindFileData->cFileName, it2->d_name);
        FindResult *pResult = new FindResult;
        pResult->it = readdir(dir2);
        pResult->dir = dir2;
        pResult->szWildcard = strdup(wildcard.c_str());
        pResult->fullpath = fullpath;
        printf("FindFirstFileA: %s\n", lpFindFileData->cFileName);
        HANDLE handle = HANDLE_Create(Handle_FindFile, pResult);
        return handle;
      }
    }
  }
  return NULL;
}
bool FindNextFileRecursive(FindResult* pResult, WIN32_FIND_DATA*
lpFindFileData);
bool FindNextFileA(
    /*in*/ HANDLE hFindFile, // handle to search
    /*out*/ WIN32_FIND_DATA *lpFindFileData // pointer to information about a
    // found file/directory
) {
  if (hFindFile == NULL)
    return false;
  FindResult *pResult = (FindResult *) hFindFile->ptr;
  while (pResult->it != NULL) {
    char fullpath[1024];
    strcpy(fullpath, pResult->fullpath.c_str());
    strcat(fullpath, "/");
    strcat(fullpath, pResult->it->d_name);
    if (strcmp(pResult->it->d_name, ".") == 0 || strcmp(pResult->it->d_name,
                                                        "..") == 0) {

      pResult->it = readdir(pResult->dir);
      continue;
    }
    /*if (pResult->it->d_type == DT_DIR) {
      if (FindNextFileRecursive(pResult, lpFindFileData)) {
        return true;
      }
    }*/
    if (boost::filesystem::is_regular_file(fullpath) && FilenameMatchesWildcard
    (fullpath,
                                pResult->szWildcard)) {
      lpFindFileData->dwFileAttributes =
          GetFileAttributesA(fullpath);
      lpFindFileData->nFileSizeHigh = 0;
      lpFindFileData->nFileSizeLow =
          boost::filesystem::file_size(fullpath);
      strcpy(lpFindFileData->cFileName,
             pResult->it->d_name);
      printf("FindNextFileA: %s\n", lpFindFileData->cFileName);
      pResult->it = readdir(pResult->dir);
      return true;
    }
    pResult->it = readdir(pResult->dir);
  }
  return false;
}
bool FindNextFileRecursive(FindResult* pResult, WIN32_FIND_DATA*
lpFindFileData) {
  if (pResult->it == NULL)
    return false;
  char fullpath[1024];
  strcpy(fullpath, pResult->fullpath.c_str());
  strcat(fullpath, "/");
  strcat(fullpath, pResult->it->d_name);
  // assume we're starting with pResult pointing to a directory, so we must initialize a new pResult
  if (pResult->it->d_type == DT_DIR) {
    DIR* dir = opendir(fullpath);
    dirent* it = readdir(dir);
    FindResult* pResult2 = new FindResult;
    pResult2->it = it;
    pResult2->dir = dir;
    pResult2->szWildcard = strdup(pResult->szWildcard);
    pResult2->fullpath = fullpath;
    if (FindNextFileRecursive(pResult2, lpFindFileData)) {
      delete pResult2;
      return true;
    }
    delete pResult2;
  }
  pResult->it = readdir(pResult->dir);
  while (pResult->it != NULL) {
    strcpy(fullpath, pResult->fullpath.c_str());
    strcat(fullpath, "/");
    strcat(fullpath, pResult->it->d_name);
    if (strcmp(pResult->it->d_name, ".") == 0 || strcmp(pResult->it->d_name,
                                                        "..") == 0) {
      pResult->it = readdir(pResult->dir);
      continue;
    }
    if (pResult->it->d_type == DT_DIR) {
      pResult->it = readdir(pResult->dir);
      if (FindNextFileRecursive(pResult, lpFindFileData)) {
        return true;
      }
    }
    if (boost::filesystem::is_regular_file(fullpath) && FilenameMatchesWildcard
    (fullpath,
                                pResult->szWildcard)) {
      lpFindFileData->dwFileAttributes =
          GetFileAttributesA(fullpath);
      lpFindFileData->nFileSizeHigh = 0;
      lpFindFileData->nFileSizeLow =
          boost::filesystem::file_size(fullpath);
      strcpy(lpFindFileData->cFileName,
             pResult->it->d_name);
      printf("FindNextFileA: %s\n", lpFindFileData->cFileName);
      pResult->it = readdir(pResult->dir);
      return true;
    }
    pResult->it = readdir(pResult->dir);
  }
  return false;
}

HANDLE FindFirstFileW(
    /*in*/ const wchar_t *lpFileName, /* NOTE: The filename can include
                                    * wildcard characters, such as an
                                    * asterisk or a question mark.
                                    */
    /*out*/ WIN32_FIND_DATAW *lpFindFileData // pointer to information about a
    // found file/directory
) {
  char szFileName[512];
  wcstombs(szFileName, lpFileName, wcslen(lpFileName) + 1);
  szFileName[wcslen(lpFileName)] = '\0';
  WIN32_FIND_DATA tmp;
  HANDLE hndl = FindFirstFileA(szFileName, &tmp);
  if (hndl != NULL) {

    memcpy(lpFindFileData, &tmp, sizeof(tmp) - 260);
    mbstowcs(lpFindFileData->cFileName, tmp.cFileName, strlen(tmp.cFileName));
  }
  return hndl;
}
bool FindNextFileW(
    /*in*/ HANDLE hFindFile, // handle to search
    /*out*/ WIN32_FIND_DATAW *lpFindFileData // pointer to information about a
    // found file/directory
) {
  WIN32_FIND_DATA tmp;
  bool succ = FindNextFileA(hFindFile, &tmp);
  if (!succ) return false;
  memcpy(lpFindFileData, &tmp, sizeof(tmp) - 260);
  mbstowcs(lpFindFileData->cFileName, tmp.cFileName, strlen(tmp.cFileName));
  return true;

}
bool FindClose(HANDLE hFindFile) {
  if (hFindFile == NULL)
    return true;
  FindResult *pResult = (FindResult *) hFindFile->ptr;
  delete pResult;
  return true;
}
enum CSIDL {
  CSIDL_ADMINTOOLS = 0x0030,
  CSIDL_ALTSTARTUP = 0x001d,
  CSIDL_APPDATA = 0x001a,
  CSIDL_BITBUCKET = 0x000a,
  CSIDL_CDBURN_AREA = 0x003b,
  CSIDL_COMMON_ADMINTOOLS = 0x002f,
  CSIDL_COMMON_ALTSTARTUP = 0x001e,
  CSIDL_COMMON_APPDATA = 0x0023,
  CSIDL_COMMON_DESKTOPDIRECTORY = 0x0019,
  CSIDL_COMMON_DOCUMENTS = 0x002e,
  CSIDL_COMMON_FAVORITES = 0x001f,
  CSIDL_COMMON_MUSIC = 0x0035,
  CSIDL_COMMON_OEM_LINKS = 0x003a,
  CSIDL_COMMON_PICTURES = 0x0036,
  CSIDL_COMMON_PROGRAMS = 0X0017,
  CSIDL_COMMON_STARTMENU = 0x0016,
  CSIDL_COMMON_STARTUP = 0x0018,
  CSIDL_COMMON_TEMPLATES = 0x002d,
  CSIDL_COMMON_VIDEO = 0x0037,
  CSIDL_COMPUTERSNEARME = 0x003d,
  CSIDL_CONNECTIONS = 0x0031,
  CSIDL_CONTROLS = 0x0003,
  CSIDL_COOKIES = 0x0021,
  CSIDL_DESKTOP = 0x0000,
  CSIDL_DESKTOPDIRECTORY = 0x0010,
  CSIDL_DRIVES = 0x0011,
  CSIDL_FAVORITES = 0x0006,
  CSIDL_FLAG_CREATE = 0x8000,
  CSIDL_FLAG_DONT_VERIFY = 0x4000,
  CSIDL_FLAG_MASK = 0xFF00,
  CSIDL_FLAG_NO_ALIAS = 0x1000,
  CSIDL_FLAG_PER_USER_INIT = 0x0800,
  CSIDL_FONTS = 0x0014,
  CSIDL_HISTORY = 0x0022,
  CSIDL_INTERNET = 0x0001,
  CSIDL_INTERNET_CACHE = 0x0020,
  CSIDL_LOCAL_APPDATA = 0x001c,
  CSIDL_MYDOCUMENTS = 0x000c,
  CSIDL_MYMUSIC = 0x000d,
  CSIDL_MYPICTURES = 0x0027,
  CSIDL_MYVIDEO = 0x000e,
  CSIDL_NETHOOD = 0x0013,
  CSIDL_NETWORK = 0x0012,
  CSIDL_PERSONAL = 0x0005,
  CSIDL_PRINTERS = 0x0004,
  CSIDL_PRINTHOOD = 0x001b,
  CSIDL_PROFILE = 0x0028,
  CSIDL_PROGRAM_FILES = 0x0026,
  CSIDL_PROGRAM_FILES_COMMON = 0x002b,
  CSIDL_PROGRAM_FILES_COMMONX86 = 0x002c,
  CSIDL_PROGRAM_FILESX86 = 0x002a,
  CSIDL_PROGRAMS = 0x0002,
  CSIDL_RECENT = 0x0008,
  CSIDL_RESOURCES = 0x0038,
  CSIDL_RESOURCES_LOCALIZED = 0x0039,
  CSIDL_SENDTO = 0x0009,
  CSIDL_STARTMENU = 0x000b,
  CSIDL_STARTUP = 0x0007,
  CSIDL_SYSTEM = 0x0025,
  CSIDL_SYSTEMX86 = 0x0029,
  CSIDL_TEMPLATES = 0x0015,
  CSIDL_WINDOWS = 0x0024
};

uint32_t SHGetFolderPathA(
    /*in*/ void *hwnd, // reserved
    /*in*/ int csidl, // identifies the folder whose path is to be retrieved
    /*in*/ void *hToken, // an access token which can be used to represent a
    // particular user (usually null)
    /*in*/ int dwFlags, // ignored
    /*out*/ char *pszPath // the output folder path
) {
  /*
   * Paths used: CSIDL_BITBUCKET, CSIDL_FONTS,
   * CSIDL_DESKTOPDIRECTORY, CSIDL_COMMON_DESKTOPDIRECTORY,
   * CSIDL_APPDATA, CSIDL_COMMON_APPDATA, CSIDL_PERSONAL,
   * CSIDL_COMMON_DOCUMENTS, CSIDL_MYMUSIC, CSIDL_COMMON_MUSIC,
   * CSIDL_PROGRAMS, CSIDL_COMMON_PROGRAMS
   * */
  std::string path;
  std::string home = getenv("HOME");
  switch (csidl) {
  case CSIDL_BITBUCKET:path = std::string("/tmp");
    break;
  case CSIDL_FONTS:path = std::string("/usr/share/fonts");
    break;
  case CSIDL_DESKTOPDIRECTORY:
  case CSIDL_COMMON_DESKTOPDIRECTORY:path = std::string(home + "/Desktop/");
    /*std::transform(path.begin(), path.end(), path.begin(), tolower);*/
    break;
  case CSIDL_APPDATA:
  case CSIDL_COMMON_APPDATA:path = std::string(home + "/.scion/AppData/");
    //std::transform(path.begin(), path.end(), path.begin(), tolower);
    if (!boost::filesystem::exists(path)) {
      mkdirpwrapper(path);
    }
    break;
  case CSIDL_PERSONAL:
  case CSIDL_COMMON_DOCUMENTS:path = std::string(home + "/.scion/Documents/");
    //std::transform(path.begin(), path.end(), path.begin(), tolower);
    if (!boost::filesystem::exists(path)) {
      mkdirpwrapper(path);
    }
    break;
  case CSIDL_MYMUSIC:
  case CSIDL_COMMON_MUSIC:path = std::string(home + "/Music/");
    break;
  case CSIDL_PROGRAMS:
  case CSIDL_COMMON_PROGRAMS:path = std::string(home + "/.scion/Programs/");
    //std::transform(path.begin(), path.end(), path.begin(), tolower);
    if (!boost::filesystem::exists(path)) {
      mkdirpwrapper(path);
    }
    break;
  default:return 1;
  }
  strcpy(pszPath, path.c_str());
  return 0;
}

uint32_t SHGetFolderPathW(
    /*in*/ void *hwnd, // reserved
    /*in*/ int csidl, // identifies the folder whose path is to be retrieved
    /*in*/ void *hToken, // an access token which can be used to represent a
    // particular user (usually null)
    /*in*/ int dwFlags, // ignored
    /*out*/ wchar_t *pszPath // the output folder path
) {
  char path[1024];
  uint32_t ret = SHGetFolderPathA(hwnd, csidl, hToken, dwFlags, path);
  mbstowcs(pszPath, path, strlen(path));
  pszPath[strlen(path) + 1] = L'\0';
  return ret;
}

/*
 * Sets the physical file size for the specified file to the current position
 * of the file pointer.
 */
bool SetEndOfFile(HANDLE hFile) {
  if (hFile == NULL) {
    return false;
  }
  FILE *fp = (FILE *) hFile->ptr;
  if (fp == NULL) {
    return false;
  }
  fflush(fp);
  int fd = fileno(fp);
  if (fd == -1) {
    return false;
  }
  return ftruncate(fd, ftell(fp)) == 0;
}
DWORD SetFilePointer(HANDLE hFile,
                     int32_t lDistanceToMove,
                     int32_t *lpDistanceToMoveHigh,
                     uint32_t dwMoveMethod) {
  if (hFile == NULL) {
    return -1;
  }
  FILE *fp = (FILE *) hFile->ptr;
  if (fp == NULL) {
    return -1;
  }
  if (lpDistanceToMoveHigh != NULL) return -1; // I don't know what to do
  // with >2GB files
  int seek_type = 0;
  switch (dwMoveMethod) {
  case 0: seek_type = SEEK_SET; break;
  case 1: seek_type = SEEK_CUR; break;
  case 2: seek_type = SEEK_END; break;
  default: break;
  }
  fseek(fp, lDistanceToMove, seek_type);
  return ftell(fp);
}

void __Z9SplitpathPKcPcS1_S1_S1_(char* path, char* drive, char* dir, char* fname, char* ext) {
  if (!path) return;
  path = (char*)DOSPathToUnixPath(path);

  printf("Splitting: %s\n", path);
  if (drive) {
    strcpy(drive, "/./");
    printf("Drive is: %s\n", drive);
  }

  if (dir) {
    char tmp[1024];
    strcpy(tmp, path);
    char* tmp2 = strrchr(tmp, '/');
    if (tmp2) {
      *(tmp2+1) = 0;
    }
    strcpy(dir, tmp);
    printf("Dir is: %s\n", dir);
  }

  if (fname) {

    char tmp[1024];
    strcpy(tmp, path);
    char* tmp2 = strrchr(tmp, '/');
    char* tmp3 = strchr(tmp2 ? tmp2 : tmp, '.');
    if (tmp3) {
      *tmp3 = 0;
    }
    if (tmp2) {
      strcpy(fname, tmp2+1);
    }
    else {
      strcpy(fname, "");
    }

    printf("Fname is: %s\n", fname);
  }

  if (ext) {
    char tmp[1024];
    strcpy(tmp, path);
    char* tmp2 = strrchr(tmp, '/');
    char* tmp3 = strchr(tmp2 ? tmp2 : tmp, '.');
    if (tmp3) {
      strcpy(ext, tmp3);
    }
    else {
      strcpy(ext, "");
    }

    printf("Ext is: %s\n", ext);
  }
}

