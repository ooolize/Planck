/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */

#pragma once
#include "timer.h"
namespace planck {

class TimerManager {
 public:
  TimerManager() = default;
  size_t addTimer(planck::Timer timer);
  void removeTimer(planck::Timer timer);

  void run();

 private:
  NanoTime calcAdvanceTime();
  NanoTime getNextTime();
  void update(NanoTime time);
  planck::Timer _timer;
  float _rate;
  GHz _cpu_frequency;
};

}  // namespace planck