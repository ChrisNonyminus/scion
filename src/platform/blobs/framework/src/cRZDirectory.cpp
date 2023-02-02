#include <cstring>
#include <cstdio>
#include "cRZDirectory.h"

#ifdef __linux__
#include <sys/stat.h>
#include <dirent.h>
#include <fnmatch.h>
#endif

// TODO: field_1C is probably a struct containing the DIR.
struct tmp_struc {
  uint8_t gap[0x4];
  const char* dirname;
  int gap1;
  DIR* dir;
};
bool RZFileFindNext(RZFileFindData *data) {
  /*if (strstr(data->field_0.Data(), "ouiav")) {
    data->field_0.replace(0, 5, "Game/"); // fuck it, no idea where this
    // "ouiav" is coming from
    closedir((DIR*)((uint8_t*)((tmp_struc*)data->field_1C)->dir));
    (((tmp_struc*)data->field_1C)->dir) = opendir(data->field_0.Data());
  }*/
#ifdef  __linux__
  dirent* ent;
  const char* curfile = NULL;
  do {
    ent = readdir((DIR*)(((tmp_struc*)data->field_1C)->dir));
    if (!ent)
      return false;
    curfile = ent->d_name;
  } while (fnmatch(data->field_10.Data(), curfile, 16) != 0);
  cRZString theFile(curfile);
  cRZString fullpath((((tmp_struc*)data->field_1C)->dirname));
  data->field_0.assign(theFile.Data());
  fullpath.Append((data->field_0));
  struct stat buf;
  if (stat(fullpath.Data(), &buf) != 0) {
    return false;
  }
  data->field_C = S_ISDIR(buf.st_mode);
  return true;
#endif
  return false;
}
#define SZ_DEFAULT_FILTER "*"


cRZDirectory::cRZDirectory(const cRZString &dirname,
                           bool a3,
                           bool a4,
                           bool a5) {
  m_iNumEntries = 0;
  field_C = dirname;
  if (strstr(dirname.Data(), "ouiav")) {
    field_C.replace(0, 5, "Game/"); // fuck it, no idea where this
    // "ouiav" is coming from
  }
  printf("Initializing cRZDirectory %s...\n", field_C.Data());
  m_rzsFilter = SZ_DEFAULT_FILTER;
  field_24 = a4;
  field_25 = a5;
  MakeSurePathEndsWithSeparator();
  if (a3) ReadDirectoryEntries();
}
