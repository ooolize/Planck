/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-28
 * @LastEditors: lize
 */

#pragma once
#include "thread_pool.h"
#include "timer_manager.h"
namespace planck {
class Locator {
 public:
  static ThreadPool& getThreadPool() {
    return _thread_pool;
  }
  static TimerManager& getTimerManager() {
    return _timer_manager;
  }

 private:
  static ThreadPool _thread_pool;
  static TimerManager _timer_manager;
};



}  // namespace planck