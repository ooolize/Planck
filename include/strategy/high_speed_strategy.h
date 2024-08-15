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
  void strategy(const Timer& current_timer) override;

 private:
  std::size_t _before_wake_us = 30 * 1000 * 1000;
};
}  // namespace planck