/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-09
 * @LastEditors: lize
 */

#include "timer_manager.h"

#include <sched.h>

#include <cassert>
#include <thread>

#include "utils/time.h"
namespace planck {

Timer& TimerManager::getCurrentTimer() {
  return _current_timer;
}

ID TimerManager::addTimer(const std::string& time_point,
                          int time_interval,
                          int repeat,
                          CallBack callback,
                          ControlStgSPtr control_stg) {
  TimeStampNs target_time = lz::timeToNanoseconds(time_point);
  auto curr_time = lz::getTimeStampNs();
  Timer timer(
    target_time - curr_time, time_interval, repeat, callback, control_stg);
  timer._id = ++timer_id;
  return addTimer(std::move(timer));
}

ID TimerManager::addTimer(TimeStampNs offset,
                          int time_interval,
                          int repeat,
                          CallBack callback,
                          ControlStgSPtr control_stg) {
  Timer timer(offset, time_interval, repeat, callback, control_stg);
  timer._id = ++timer_id;
  return addTimer(std::move(timer));
}

ID TimerManager::addTimer(planck::Timer&& timer) {  // NOLINT
  std::size_t count = 0;

  while (++count < timer._repeat) {
    auto new_timer = timer;
    auto interval_offset =
      lz::nanoTime2rdtsc(timer._time_interval, Timer::_frequence);
    new_timer._id = ++timer_id;
    new_timer._repeat = 0;
    new_timer._rdtsc_timestamp_plan_wake =
      timer._rdtsc_timestamp_plan_wake + interval_offset * count;
    _timer_container.insert(std::move(new_timer));
  }
  _timer_container.insert(std::move(timer));
  _current_timer = timer;
  return timer_id;
}

void TimerManager::removeTimer(const planck::Timer& timer) {
  auto result = _timer_container.find(timer);
  if (!result) [[unlikely]] {
    return;
  }
  _timer_container.remove(result->_value);
}
void TimerManager::start() {
  _start = true;
  std::thread t(&TimerManager::run, this);
  t.join();
}

void TimerManager::stop() {
  _start = false;
  // TODO() 同步
}

void TimerManager::run() {
  // int cpu = sched_getcpu();  // 获取当前线程所在的 CPU 核心编号
  // std::cout << "Thread running on CPU: " << cpu << std::endl;
  while (1) {
    if (!_timer_container.size()) {
      // std::this_thread::sleep_for(std::chrono::milliseconds(1));
      // std::cout << " empty " << std::endl;
      break;
    }
    _current_timer._control_stg->strategy(_current_timer);
    removeTimer(_current_timer);

    auto min_node = _timer_container.findMin();
    if (!min_node) {
      continue;
    }
    _current_timer = min_node->_value;
  }
}
}  // namespace planck