/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-28
 * @LastEditors: lize
 */
#include "thread_pool.h"

#include <chrono>
namespace planck {

ThreadPool::ThreadPool(int thread_num) {
  for (int i = 0; i < thread_num; ++i) {
    auto task = [this] {
      while (!_exit) {
        _cv.wait_for(_lock, std::chrono::seconds(1), [this] {
          return _tasks.size_approx() > 0;
        });
        CallBack task;
        if (_tasks.try_dequeue(task)) {
          task();
        }
      }
    };
    _threads.emplace_back(std::move(task));
  }
};

void ThreadPool::dispatch(CallBack&& task) {
  _tasks.enqueue(std::move(task));
  _cv.notify_one();
}
}  // namespace planck