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
void HighSpeedControlStg::strategy(Timer& current_timer) {
  auto sleep_time = current_timer.getSleepTime();
  std::cout << "getSleepTime: " << sleep_time << std::endl;
  NanoTime unit_sleep_time = 0;
  // #ifdef DEBUG
  //   auto before_sleep = lz::rdtscp();
  // #endif
  // std::cout << "sleep time: " << sleep_time - _before_wake_us << std::endl;
  // first sleep
  // std::this_thread::sleep_for(
  //   std::chrono::nanoseconds(sleep_time - _before_wake_us));
  // std::this_thread::sleep_for(std::chrono::nanoseconds(900 * 1000 * 1000));
  while (sleep_time > _before_wake_us) {
    unit_sleep_time = sleep_time >> 3;
    // need do some thing .otherwise os will not schedule the thread
    lz::rdtscp();
    std::this_thread::sleep_for(
      std::chrono::nanoseconds(sleep_time - unit_sleep_time));
    // std::cout << "sleep_time: " << sleep_time << std::endl;
    // std::cout << "unit_sleep_time: " << unit_sleep_time << std::endl;
    // std::cout << "spend time: " << lz::spendTimeNs(start, end, 3.69306)
    //           << std::endl;
    sleep_time = unit_sleep_time;
  }

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
  std::size_t current_rdtsc = 0;
  while (current_rdtsc < current_timer.getRdtscTime()) {
    current_rdtsc = lz::rdtscp();
  };
  // current_timer._rdtsc_timestamp_real_start = current_rdtsc;
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
  // std::cout << "real wake time: " << current_rdtsc << std::endl;
  // std::cout << "plan wake time: " << current_timer.getRdtscTime() <<
  // std::endl;
  // std::cout << "real wake time - plan wake time: "
  //           << current_rdtsc - current_timer.getRdtscTime() << std::endl;
  current_timer.OnTimer();
  return;
}
}  // namespace planck