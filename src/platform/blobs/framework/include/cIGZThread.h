#ifndef BLOBS_FRAMEWORK_INCLUDE_CIGZTHREAD_H
#define BLOBS_FRAMEWORK_INCLUDE_CIGZTHREAD_H

#include <stdint.h>
struct cIGZThreadStartable;
struct cIGZThread {
  virtual void Release() = 0;
  virtual void Begin(uint32_t(*callback)(void*), void* userData) = 0;
  virtual void Begin(cIGZThreadStartable* startable) = 0;
  virtual void End(long) = 0;
  virtual void Suspend() = 0;
  virtual void Resume() = 0;
  virtual void Sleep(unsigned long) = 0;
  virtual void Wake() = 0;
  virtual void Join(unsigned long) = 0;
  virtual unsigned long GetPriority() = 0;
  virtual void SetPriority(unsigned long) = 0;
  virtual bool IsValid() = 0;
  virtual void SetThreadName(char*) = 0;
  virtual ~cIGZThread() = 0;
  virtual void GetExitStatus(unsigned long&) = 0;
  virtual void GetThreadID(unsigned long&) = 0;
  virtual void GetThreadHandle(void*) = 0;
};

#endif //BLOBS_FRAMEWORK_INCLUDE_CIGZTHREAD_H
