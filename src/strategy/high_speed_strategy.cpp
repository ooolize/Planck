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
  std::cout << "set sleep time: " << sleep_time << std::endl;
  NanoTime unit_sleep_time = sleep_time / _convergence_coefficent;
  NanoTime start = 0;
  NanoTime end = 0;
  NanoTime real_sleep_time = 0;
  while (unit_sleep_time > _before_wake && sleep_time > 0) {
    // need do some thing .otherwise os will not schedule the thread
    lz::rdtscp();

    start = lz::rdtscp();
    std::this_thread::sleep_for(
      std::chrono::nanoseconds(sleep_time - unit_sleep_time));
    end = lz::rdtscp();
    std::cout << "sleep time:"
              << lz::spendTimeNs(start, end, current_timer._frequence)
              << "plan time:" << sleep_time - unit_sleep_time << std::endl;

    // sleep_time = unit_sleep_time * (1.1 / _convergence_coefficent - 0.1);
    real_sleep_time = lz::spendTimeNs(start, end, current_timer._frequence);
    if (real_sleep_time > sleep_time) {
      break;
    }
    sleep_time = sleep_time - real_sleep_time;
    unit_sleep_time = sleep_time / _convergence_coefficent;
    // std::cout << "sleep_time: " << sleep_time << std::endl;
    std::cout << " unit_sleep_time: " << unit_sleep_time << std::endl;
  }

  // busy wait rdstc
  std::size_t current_rdtsc = 0;
  while (current_rdtsc < current_timer._rdtsc_timestamp_plan_wake) {
    current_rdtsc = lz::rdtscp();
  };

  current_timer.OnTimer();

  return;
}
}  // namespace planck