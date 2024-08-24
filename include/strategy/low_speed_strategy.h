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
  LowSpeedControlStg() = default;
  void strategy(Timer& current_timer) override {
    std::this_thread::sleep_for(
      std::chrono::nanoseconds(current_timer.DurationCurrToWakeup()));
  }
};
}  // namespace planck