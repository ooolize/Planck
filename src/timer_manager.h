/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */

#pragma once
#include <sched.h>

#include <cassert>
#include <memory>
#include <queue>
#include <string>
#include <thread>

#include "interface/control_stg.h"
#include "timer.h"
#include "utils/rbtree.h"
#include "utils/system.h"
#include "utils/time.h"
namespace planck {

class TimerManager {
 public:
  TimerManager() = default;
  ~TimerManager();
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
  void exit();

  Timer& getCurrentTimer();

 private:
  void run();

 private:
  std::vector<int> v;
  bool _start = false;
  bool _is_exit = false;

  Timer _current_timer{};
  lz::rbtree::RBTree<Timer> _timer_container{};
  std::jthread _thread{};
};

}  // namespace planck