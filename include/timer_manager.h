/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */

#pragma once
#include <memory>

#include "interface/control_stg.h"
#include "strategy/high_speed_strategy.h"
#include "strategy/low_speed_strategy.h"
#include "timer.h"
namespace planck {
using ControlStgSPtr = std::shared_ptr<ControlStg>;
using ControlStgUPtr = std::unique_ptr<ControlStg>;

static float frequence = lz::getFrequencyGHz();
class TimerManager {
 public:
  explicit TimerManager(bool is_low_precision = false)
    : low_precision(is_low_precision) {
    if (low_precision)
      _control_stg = std::make_unique<LowSpeedControlStg>(_timer);
    else
      _control_stg = std::make_unique<HighSpeedControlStg>(_timer);
    _cpu_frequency = lz::getFrequencyGHz();
  }
  std::size_t addTimer(planck::Timer timer);
  void removeTimer(planck::Timer timer);

  void run();

 private:
  bool low_precision = false;
  ControlStgUPtr _control_stg;
  planck::Timer _timer;
  GHz _cpu_frequency;
};

}  // namespace planck