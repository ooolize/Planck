/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-09
 * @LastEditors: lize
 */

#include "timer_manager.h"

#include "locator.h"
namespace planck {

TimerManager::~TimerManager() {
  if (_start) {
    exit();
    Locator::getTaskPool().exit();
  }
}

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
  return addTimer(std::move(timer));
}

ID TimerManager::addTimer(TimeStampNs offset,
                          int time_interval,
                          int repeat,
                          CallBack callback,
                          ControlStgSPtr control_stg) {
  Timer timer(offset, time_interval, repeat, callback, control_stg);
  return addTimer(std::move(timer));
}

ID TimerManager::addTimer(planck::Timer&& timer) {  // NOLINT
  std::size_t count = 0;
  auto plan_wake = timer._rdtsc_timestamp_plan_wake;
  while (++count < timer._repeat) {
    auto new_timer = timer;
    auto interval_offset =
      lz::nanoTime2rdtsc(timer._time_interval, Timer::_frequence);
    new_timer._repeat = 0;
    new_timer._rdtsc_timestamp_plan_wake = plan_wake + interval_offset * count;
    _timer_container.insert(std::move(new_timer));
  }
  _timer_container.insert(std::move(timer));
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (plan_wake < _current_timer._rdtsc_timestamp_plan_wake) {
      _is_cut.store(true);
      _cv.notify_one();
    }
  }

  return plan_wake;
}

std::optional<Timer> TimerManager::findTimer(ID id) {
  auto result = _timer_container.find(Timer(id));
  if (!result) [[unlikely]] {
    return std::nullopt;
  }
  return result->_value;
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
  _thread = std::jthread(&TimerManager::run, this);
}

void TimerManager::stop() {
  _start = false;
}

void TimerManager::exit() {
  stop();
  _is_exit = true;
}

void TimerManager::setCPU(std::size_t cpu) {
  _cpu = cpu;
  lz::system::setCPUAffinity(_cpu);
  Locator::getTaskPool().setCPU(_cpu);
  Locator::getTaskPool().start();
}

void TimerManager::run() {
  lz::rbtree::RBTree<planck::Timer>::NodeSPtr min_node = {};
  // int cpu = sched_getcpu();  // 获取当前线程所在的 CPU 核心编号
  // std::cout << "Thread running on CPU: " << cpu << std::endl;
  std::cout << "run thread id = " << lz::system::gettid() << std::endl;
  while (!_is_exit) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    while (_start) {
      if (!_timer_container.size()) {
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // std::cout << " empty " << std::endl;
        continue;
      }

      min_node = _timer_container.findMin();

      if (!min_node) {
        continue;
      }
      {
        std::lock_guard<std::mutex> lock(_mutex);
        _current_timer = min_node->_value;
      }
      _current_timer._control_stg->strategy(_current_timer);
      if (_is_cut.load()) {
        _is_cut.store(false);
        continue;
      }
      removeTimer(_current_timer);
    }
  }
}
}  // namespace planck