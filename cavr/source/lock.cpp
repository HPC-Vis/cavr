#include <cavr/lock.h>

namespace cavr {

Lock::Lock() {
  pthread_rwlock_init(&lock_, nullptr);
}

void Lock::readLock() {
  pthread_rwlock_rdlock(&lock_);
}

void Lock::readUnlock() {
  pthread_rwlock_unlock(&lock_);
}

void Lock::writeLock() {
  pthread_rwlock_wrlock(&lock_);
}

void Lock::writeUnlock() {
  pthread_rwlock_unlock(&lock_);
}

Lock::~Lock() {
  pthread_rwlock_destroy(&lock_);
}

} // namespace cavr
