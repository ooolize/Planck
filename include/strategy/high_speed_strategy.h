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
  std::size_t _before_wake = 100 * 1000;
  std::size_t _convergence_coefficent = 8;
};
}  // namespace planck