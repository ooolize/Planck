/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */

#pragma once
#include <sched.h>

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <utility>

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

  std::optional<Timer> findTimer(ID id);
  void removeTimer(const planck::Timer& timer);
  void start();
  void stop();
  void exit();

  Timer& getCurrentTimer();
  auto& getCut() {
    return _is_cut;
  }
  auto& getcv() {
    return _cv;
  }
  auto& getMutex() {
    return _mutex;
  }

  void setCPU(std::size_t cpu);

 private:
  void run();

 private:
  std::vector<int> v;
  std::atomic<bool> _start = false;
  std::atomic<bool> _is_exit = false;
  std::atomic<bool> _is_cut = false;
  std::condition_variable _cv{};
  std::mutex _mutex{};
  std::size_t _cpu = 5;
  Timer _current_timer{};
  lz::rbtree::RBTree<Timer> _timer_container{};
  std::jthread _thread{};
};

}  // namespace planck