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
  explicit TimerManager(bool is_low_precision = false);

  ID addTimer(planck::Timer&& timer);
  void removeTimer(ID id);
  void start();
  void stop();

  void run();

 private:
  bool _low_precision = false;
  bool _start = false;
  Timer _current_timer{};
  // ControlStgUPtr _control_stg;
  lz::rbtree::RBTree<Timer> _timer_container{};
};

}  // namespace planck