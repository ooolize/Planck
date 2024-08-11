/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-11
 * @LastEditors: lize
 */
#pragma once
#include <iostream>
#include <thread>

#include "interface/control_stg.h"
#include "timer.h"
namespace planck {

class HighSpeedControlStg : public ControlStg {
 public:
  explicit HighSpeedControlStg(TimerSPtr timer) : _timer(timer) {
  }
  void strategy() override {
    // first sleep
    std::this_thread::sleep_for(
      std::chrono::nanoseconds(_timer->getSleepTime() - _before_wake_us));
    // before the timer should be waked up.busy query rdstc
    while (lz::rdtsc() < _timer->getRdtscTime()) {
    }
    _timer->OnTimer();
  }

 private:
  TimerSPtr _timer;
  std::size_t _before_wake_us = 30;
};
}  // namespace planck