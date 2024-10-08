/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-28
 * @LastEditors: lize
 */
#include "task_pool.h"

#include <chrono>

#include "utils/system.h"
namespace planck {

TaskPool::~TaskPool() {
  exit();
}

void TaskPool::exit() {
  _exit = true;
  _cv.notify_all();
}

void TaskPool::setCPU(std::size_t cpu) {
  _cpu = cpu;
}

TaskPool::TaskPool(int thread_num) {
  _thread_num = thread_num;
};

void TaskPool::start() {
  for (int i = 0; i < _thread_num; ++i) {
    auto task = [this] {
      lz::system::setCPUAffinity(_cpu);
      CallBack task;
      while (!_exit) {
        std::unique_lock<std::mutex> _lock{_mutex};
        _cv.wait_for(_lock, std::chrono::seconds(1), [this] {
          return _exit || _tasks.size_approx() > 0;
        });
        // std::this_thread::sleep_for(std::chrono::microseconds(1));
        if (!_exit && _tasks.try_dequeue(task)) {
          task();
        }
      }
    };
    _threads.emplace_back(std::move(task));
  }
}

void TaskPool::dispatch(CallBack&& task) {
  _tasks.enqueue(std::move(task));
  _cv.notify_one();
}

}  // namespace planck