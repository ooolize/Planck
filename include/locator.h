/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-28
 * @LastEditors: lize
 */

#pragma once
#include "task_pool.h"
#include "timer_manager.h"
namespace planck {
class Locator {
 public:
  static TaskPool& getTaskPool() {
    return _thread_pool;
  }
  static TimerManager& getTimerManager() {
    return _timer_manager;
  }
  static double getFrequenceGHz() {
    return _frequence;
  }

 private:
  static TaskPool _thread_pool;
  static TimerManager _timer_manager;
  static double _frequence;
};

}  // namespace planck