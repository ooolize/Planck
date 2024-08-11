/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-09
 * @LastEditors: lize
 */

#include "timer_manager.h"

#include <math.h>

#include <cassert>
#include <thread>

#include "utils/time.h"
namespace planck {

size_t TimerManager::addTimer(planck::Timer timer) {
  _timer = timer;
  return 0;
}

void TimerManager::removeTimer(planck::Timer id) {
}

void TimerManager::update(NanoTime rdtsc_time) {
  assert(rdtsc_time >= 0);
  auto real_time = lz::rdtsc2nanoTime(rdtsc_time, _cpu_frequency);
  std::this_thread::sleep_for(std::chrono::nanoseconds(real_time));
  _timer.OnTimer();
}

NanoTime TimerManager::calcAdvanceTime() {
  return _timer.getRdtscTime() - lz::rdtsc();
}

void TimerManager::run() {
  while (true) {
    auto time = calcAdvanceTime();
    update(time);
    return;
  }
}
}  // namespace planck