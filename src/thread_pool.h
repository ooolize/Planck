/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-28
 * @LastEditors: lize
 */

#pragma once
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "concurrentqueue.h"
#include "interface/define.h"

namespace planck {

class ThreadPool {
 public:
  explicit ThreadPool(int thread_num = 5);
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ~ThreadPool() = default;

  void dispatch(CallBack&& task);
  void exit() {
    _exit = true;
    // _cv.notify_all();
  }

 private:
  bool _exit = false;
  std::mutex _mutex{};
  std::unique_lock<std::mutex> _lock{_mutex};
  std::condition_variable _cv{};

  std::vector<std::jthread> _threads{};
  moodycamel::ConcurrentQueue<CallBack> _tasks{};
};

}  // namespace planck