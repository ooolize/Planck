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

void TimerManager::addTimer(planck::Timer timer) {
  _timer_container.insert(timer);
  return;
}

void TimerManager::removeTimer(planck::Timer timer) {
  _timer_container.remove(timer);
}

void TimerManager::run() {
  while (true) {
    this->_control_stg->strategy();
    return;
  }
}
}  // namespace planck