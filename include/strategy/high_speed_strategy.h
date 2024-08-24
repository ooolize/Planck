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
#include "interface/define.h"
#include "timer.h"
namespace planck {

class HighSpeedControlStg : public ControlStg {
 public:
  HighSpeedControlStg() = default;
  explicit HighSpeedControlStg(std::size_t before_wake_us);
  void strategy(Timer& current_timer) override;

 private:
  // 提前唤醒 准备busywait
  std::size_t _before_wake = 100 * 1000;
  // 越高 busywait时间越长 sleep次数越少
  static constexpr std::size_t _convergence_coefficent = 4;
  // sleep在us级别的补偿
  static constexpr std::size_t _sleep_compensation = 80 * 1000;
  static constexpr std::size_t _1ms = 1 * 1000 * 1000;
};
}  // namespace planck