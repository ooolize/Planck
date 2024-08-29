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

class TaskPool {
 public:
  explicit TaskPool(int thread_num = 5);
  TaskPool(TaskPool&&) = delete;
  TaskPool(const TaskPool&) = delete;
  TaskPool& operator=(TaskPool&&) = delete;
  TaskPool& operator=(const TaskPool&) = delete;
  ~TaskPool() {
    exit();
  };

  void dispatch(CallBack&& task);
  void exit() {
    _exit = true;
    // _cv.notify_all();
  }

 private:
  bool _exit = false;
  std::mutex _mutex{};
  std::condition_variable _cv{};
  std::vector<std::size_t> _record{};
  std::vector<std::jthread> _threads{};
  moodycamel::ConcurrentQueue<CallBack> _tasks{};
};

}  // namespace planck