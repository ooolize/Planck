/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-12
 * @LastEditors: lize
 */
#include "strategy/high_speed_strategy.h"

namespace planck {
HighSpeedControlStg::HighSpeedControlStg(std::size_t before_wake_us)
  : _before_wake_us(before_wake_us) {
}
void HighSpeedControlStg::strategy(const Timer& current_timer) {
  auto sleep_time = current_timer.getSleepTime();

  // #ifdef DEBUG
  //   auto before_sleep = lz::rdtscp();
  // #endif

  // first sleep
  std::this_thread::sleep_for(
    std::chrono::nanoseconds(sleep_time - _before_wake_us));

  // #ifdef DEBUG
  //   auto after_sleep = lz::rdtscp();
  //   std::cout << "real sleep time: "
  //             << lz::spendTimeNs(before_sleep, after_sleep, 3.69306)
  //             << "===>set timer spend time:"
  //             << lz::spendTimeNs(current_timer._rdtsc_timestamp_real_start,
  //                                after_sleep,
  //                                3.69306)
  //             << std::endl;
  // #endif

  // before the timer should be waked up.
  // busy wait rdstc
  while (lz::rdtscp() < current_timer.getRdtscTime());
  // #ifdef DEBUG
  //   auto current_rdtsc = lz::rdtscp();
  //   std::cout << "busy wait spend time: "
  //             << lz::spendTimeNs(after_sleep, current_rdtsc, 3.69306)
  //             << "===>set timer spend time:"
  //             << lz::spendTimeNs(current_timer._rdtsc_timestamp_real_start,
  //                                current_rdtsc,
  //                                3.69306)
  //             << std::endl;
  // #endif
  return;
}
}  // namespace planck