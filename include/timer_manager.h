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
#include "utils/rbtree.h"

namespace planck {
using ControlStgSPtr = std::shared_ptr<ControlStg>;
using ControlStgUPtr = std::unique_ptr<ControlStg>;

class TimerManager {
 public:
  explicit TimerManager(bool is_low_precision = false)
    : low_precision(is_low_precision) {
    if (low_precision)
      _control_stg = std::make_unique<LowSpeedControlStg>();
    else
      _control_stg = std::make_unique<HighSpeedControlStg>();
  }
  void addTimer(planck::Timer timer);
  void removeTimer(planck::Timer timer);

  void run();

 private:
  bool low_precision = false;
  ControlStgUPtr _control_stg;
  lz::RBTree<Timer> _timer_container;
};

}  // namespace planck