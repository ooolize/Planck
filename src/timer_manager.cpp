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

std::size_t TimerManager::addTimer(planck::Timer timer) {
  _timer = timer;
  return 0;
}

void TimerManager::removeTimer(planck::Timer id) {
}

void TimerManager::run() {
  while (true) {
    // auto delt_time = calcDeltTime();
    // update(delt_time);
    this->_control_stg->strategy();
    return;
  }
}
}  // namespace planck