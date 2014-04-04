#pragma once
#include <pthread.h>

namespace cavr {

class Lock {
public:
  Lock();
  void readLock();
  void readUnlock();
  void writeLock();
  void writeUnlock();
  ~Lock();
private:
  pthread_rwlock_t lock_;
};

} // namespace cavr
