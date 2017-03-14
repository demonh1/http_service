#ifndef SYNC_H
#define SYNC_H

#include <thread>
#include <mutex>
#include <condition_variable>

namespace utils {

using mutex_t = std::mutex;
using lock_t = std::unique_lock<std::mutex>;
using condvar_t = std::condition_variable;


} //namespace utils

#endif // SYNC_H
