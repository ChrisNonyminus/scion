#ifndef BLOBS_FRAMEWORK_INCLUDE_CRZTHREAD_H
#define BLOBS_FRAMEWORK_INCLUDE_CRZTHREAD_H

#include "cIGZThread.h"

struct cRZThread : public cIGZThread {
  uint8_t unk[4];
  cRZThread();

  virtual void Release();
  virtual void Begin(uint32_t(*callback)(void*), void* userData);
  virtual void Begin(cIGZThreadStartable* startable);
  virtual void End(long);
  virtual void Suspend();
  virtual void Resume();
  virtual void Sleep(unsigned long);
  virtual void Wake();
  virtual void Join(unsigned long);
  virtual unsigned long GetPriority();
  virtual void SetPriority(unsigned long);
  virtual bool IsValid();
  virtual void SetThreadName(char*);
  virtual ~cRZThread();
  virtual void GetExitStatus(unsigned long&);
  virtual void GetThreadID(unsigned long&);
  virtual void GetThreadHandle(void*);
};

#endif //BLOBS_FRAMEWORK_INCLUDE_CRZTHREAD_H
