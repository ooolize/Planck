/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-09
 * @LastEditors: lize
 */

#include "manager.h"

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
  auto real_time = lztime::rdtsc2nanoTime(rdtsc_time, _cpu_frequency);
  std::this_thread::sleep_for(std::chrono::nanoseconds(real_time));
  _timer.OnTimer();
}

NanoTime TimerManager::calcAdvanceTime() {
  return _timer.getRdtscTime() - lztime::rdtsc();
}

void TimerManager::run() {
  while (true && _timer.getStatus()) {
    auto time = calcAdvanceTime();
    update(time);
  }
}
}  // namespace planck