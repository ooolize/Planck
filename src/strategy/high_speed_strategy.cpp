/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-12
 * @LastEditors: lize
 */
#include "strategy/high_speed_strategy.h"

namespace planck {
HighSpeedControlStg::HighSpeedControlStg(std::size_t before_wake_us)
  : _before_wake(before_wake_us) {
}
void HighSpeedControlStg::strategy(Timer& current_timer) {
  auto sleep_time = current_timer.getSleepTime();
  NanoTime unit_sleep_time = sleep_time >> _convergence_coefficent;
  while (unit_sleep_time > _before_wake) {
    // need do some thing .otherwise os will not schedule the thread
    lz::rdtscp();

    std::this_thread::sleep_for(
      std::chrono::nanoseconds(sleep_time - unit_sleep_time));
    sleep_time = unit_sleep_time;
    unit_sleep_time = sleep_time >> _convergence_coefficent;
  }

  // busy wait rdstc
  std::size_t current_rdtsc = 0;
  while (current_rdtsc < current_timer.getRdtscTime()) {
    current_rdtsc = lz::rdtscp();
  };

  current_timer.OnTimer();
  return;
}
}  // namespace planck