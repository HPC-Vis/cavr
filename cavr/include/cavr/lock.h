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

class ReadLock {
public:
  ReadLock(Lock* lock);
  ~ReadLock();
private:
  Lock* lock_;
  bool released_;
};

class WriteLock {
public:
  WriteLock(Lock* lock);
  ~WriteLock();
private:
  Lock* lock_;
  bool released_;
};

} // namespace cavr
