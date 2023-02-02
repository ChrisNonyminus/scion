#ifndef BLOBS_FRAMEWORK_INCLUDE_CRZDIRECTORY_H
#define BLOBS_FRAMEWORK_INCLUDE_CRZDIRECTORY_H

#include "cRZString.h"

#include <list>

struct RZFileFindData {
  cRZString field_0;
  bool field_C;
  cRZString field_10;
  void* field_1C;
};

class cRZDirectoryEntry {
  int field_0;
  cRZString field_4;
  int field_10;
};

class cRZDirectory {
  std::list<cRZDirectoryEntry> m_entries;
  int m_iNumEntries;
  cRZString field_C;
  cRZString m_rzsFilter;
  bool field_24, field_25;
public:
  cRZDirectory(const cRZString& dirname, bool a3, bool a4, bool a5);
  void ReadDirectoryEntries();
  int GetDirectoryEntryCount();
  cRZDirectoryEntry* GetNthEntry(unsigned long idx);
  void MakeSurePathEndsWithSeparator();
  static bool RemoveDirectoryA(const cRZString& dirname);
};

void RZFileFindFinish(RZFileFindData* data);
bool RZFileFindNext(RZFileFindData* data);

#endif //BLOBS_FRAMEWORK_INCLUDE_CRZDIRECTORY_H
