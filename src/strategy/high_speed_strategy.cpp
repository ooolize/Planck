/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-12
 * @LastEditors: lize
 */
#include "strategy/high_speed_strategy.h"

#include "locator/locator.h"

namespace planck {
HighSpeedControlStg::HighSpeedControlStg(std::size_t before_wake_us)
  : _before_wake(before_wake_us) {
}
void HighSpeedControlStg::strategy(Timer& current_timer) {
  // std::size_t to_target_time = 0;
  // NanoTime unit_sleep_time = 0;
  std::size_t to_target_time = current_timer.DurationCurrToWakeup();
  NanoTime step_sleep_time = 0;
  NanoTime start = 0;
  NanoTime end = 0;
  NanoTime real_sleep_time = 0;
  while (to_target_time > _before_wake) {
    // need do some thing .otherwise os will not schedule the thread
    // lz::rdtscp();
    // 必须每次求值 不能依赖real_sleep_time
    to_target_time = current_timer.DurationCurrToWakeup();
    step_sleep_time = to_target_time - to_target_time / _convergence_coefficent;
    if (step_sleep_time < _1ms) {
      step_sleep_time -= _sleep_compensation;
    }
    start = lz::rdtscp();
    std::this_thread::sleep_for(std::chrono::nanoseconds(step_sleep_time));
    end = lz::rdtscp();
    real_sleep_time = lz::spendTimeNs(start, end, current_timer._frequence);
    // std::cout << " real sleep time:" << real_sleep_time
    //           << " plan sleep time:" << step_sleep_time
    //           << " remaind sleep time:" << to_target_time << std::endl;

    // if bias is too large , advance the time remaining
    if (real_sleep_time > to_target_time) {
      std::cout << " late" << std::endl;
      break;
    }

    // std::cout << "to_target_time: " << to_target_time << std::endl;
  }
  // end = lz::rdtscp();
  // std::cout << " end: " << end << std::endl;
  // busy wait rdstc
  std::size_t current_rdtsc = 0;
  while (current_rdtsc < current_timer._rdtsc_timestamp_plan_wake) {
    current_rdtsc = lz::rdtscp();
  };
  // end = lz::rdtscp();
  // std::cout << " end2: " << end << std::endl;
  // auto p = current_timer;
  Locator::getThreadPool().dispatch(std::move(current_timer._callback));

  return;
}
}  // namespace planck