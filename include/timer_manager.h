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

class TimerManager {
 public:
  explicit TimerManager(bool is_low_precision = false)
    : low_precision(is_low_precision) {
    // if (low_precision)
    //   _control_stg = std::make_unique<LowSpeedControlStg>();
    // else
    //   _control_stg = std::make_unique<HighSpeedControlStg>();
  }
  // must use std::move to transfer the ownership of the timer
  ID addTimer(planck::Timer&& timer);
  void removeTimer(ID id);

  void run();

 private:
  bool low_precision = false;
  Timer _current_timer;
  // ControlStgUPtr _control_stg;
  lz::rbtree::RBTree<Timer> _timer_container;
};

}  // namespace planck