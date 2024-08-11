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
class LowSpeedControlStg : public ControlStg {
 public:
  explicit LowSpeedControlStg(TimerSPtr timer = nullptr) : _timer(timer) {
  }
  void strategy() override {
    if (_timer == nullptr) {
      std::cerr << "timer is nullptr" << std::endl;
      return;
    }
    std::this_thread::sleep_for(
      std::chrono::nanoseconds(_timer->getSleepTime()));
    _timer->OnTimer();
  }

 private:
  TimerSPtr _timer{};
};
}  // namespace planck