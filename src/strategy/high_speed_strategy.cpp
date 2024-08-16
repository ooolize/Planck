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
  auto a = 1;
  auto b = 2;
  std::swap(a, b);
}
void HighSpeedControlStg::strategy(const Timer& current_timer) {
  // first sleep
  auto sleep_time = current_timer.getSleepTime();
#ifdef DEBUG
  std::cout << "HighSpeedControlStg::strategy sleep " << sleep_time
            << std::endl;
  auto before_sleep = lz::rdtscp();
#endif
  std::this_thread::sleep_for(
    std::chrono::nanoseconds(sleep_time - _before_wake_us));
// before the timer should be waked up.busy query rdstc
#ifdef DEBUG
  std::cout << "wake up from sleep has spend time: "
            << lz::spendTimeNs(before_sleep, lz::rdtscp(), 3.69306)
            << std::endl;
#endif
  while (lz::rdtscp() < current_timer.getRdtscTime());
  return;
}
}  // namespace planck