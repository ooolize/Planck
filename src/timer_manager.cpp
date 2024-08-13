/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-09
 * @LastEditors: lize
 */

#include "timer_manager.h"

#include <cassert>
#include <thread>

#include "utils/time.h"
namespace planck {

ID TimerManager::addTimer(planck::Timer&& timer) {
  timer.setId(++timer_id);
  _timer_container.insert(std::move(timer));
  return timer_id;
}

void TimerManager::removeTimer(ID id) {
  auto result = _timer_container.find(Timer(id));
  if (!result) [[unlikely]] {
    return;
  }
  _timer_container.remove(result->_value);
}

void TimerManager::run() {
  while (true) {
    _current_timer.getControlStg()->strategy(_current_timer);
    // if not remove
    auto min_timer = _timer_container.findMin()->_value;
    if (_current_timer.getId() == min_timer.getId()) {
      _current_timer.OnTimer();
      removeTimer(_current_timer.getId());
    }
    _current_timer = min_timer;
  }
}
}  // namespace planck