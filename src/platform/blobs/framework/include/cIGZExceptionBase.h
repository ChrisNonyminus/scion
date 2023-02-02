#ifndef BLOBS_FRAMEWORK_INCLUDE_CIGZEXCEPTIONBASE_H
#define BLOBS_FRAMEWORK_INCLUDE_CIGZEXCEPTIONBASE_H

#include "GZTypes.h"

class cIGZExceptionBase {
public:
  virtual const char* What() const = 0;
};

#endif //BLOBS_FRAMEWORK_INCLUDE_CIGZEXCEPTIONBASE_H
