/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */

#pragma once
#include <memory>
#include <queue>
#include <string>

#include "interface/control_stg.h"
#include "strategy/high_speed_strategy.h"
#include "strategy/low_speed_strategy.h"
#include "timer.h"
#include "utils/rbtree.h"
namespace planck {

class TimerManager {
 public:
  TimerManager() = default;

  ID addTimer(planck::Timer&& timer);

  // 指定距离 当前时刻偏移量
  ID addTimer(TimeStampNs offset,
              int time_interval,
              int repeat,
              CallBack callback,
              ControlStgSPtr control_stg = nullptr);

  // 指定当天某个时刻(hh:mm:ss 123456)
  ID addTimer(const std::string& time_point,
              int time_interval,
              int repeat,
              CallBack callback,
              ControlStgSPtr control_stg = nullptr);

  void removeTimer(const planck::Timer& timer);
  void start();
  void stop();

  void run();

 private:
  std::vector<int> v;
  bool _start = false;
  Timer _current_timer{};
  lz::rbtree::RBTree<Timer> _timer_container{};  // Ready oncall
  std::queue<Timer> _timer_queue{};              // out of 30s
};

}  // namespace planck