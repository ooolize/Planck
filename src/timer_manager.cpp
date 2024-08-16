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

TimerManager::TimerManager(bool is_low_precision)
  : _low_precision(is_low_precision) {
}

ID TimerManager::addTimer(planck::Timer timer) {
#ifdef DEBUG
  auto tmp1 = lz::rdtscp();
#endif

  timer.setId(++timer_id);
  _timer_container.insert(timer);
  // #ifdef DEBUG
  //   std::cout << "add timer cost1: "
  //             << lz::spendTimeNs(start, lz::rdtscp(), 3.69306) << std::endl;
  // #endif
  auto t = _timer_container.findMin();
  // #ifdef DEBUG
  //   std::cout << "add timer cost2: "
  //             << lz::spendTimeNs(start, lz::rdtscp(), 3.69306) << std::endl;
  // #endif
  _current_timer = t ? t->_value : Timer();
#ifdef DEBUG
  std::cout << "add timer cost " << lz::spendTimeNs(tmp1, lz::rdtscp(), 3.69306)
            << std::endl;
#endif
  return timer_id;
}

void TimerManager::removeTimer(ID id) {
  auto result = _timer_container.find(Timer(id));
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
  int cpu = sched_getcpu();  // 获取当前线程所在的 CPU 核心编号
  std::cout << "Thread running on CPU: " << cpu << std::endl;
  while (1) {
    if (!_timer_container.size()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      continue;
    }
    // #ifdef DEBUG
    //     auto start = lz::rdtscp();
    // #endif
    _current_timer.getControlStg()->strategy(_current_timer);
    // #ifdef DEBUG
    //     auto end = lz::rdtscp();
    //     std::cout << "===>after stg: set timer spend time: "
    //               << lz::spendTimeNs(
    //                    start,
    //                    end, 3.69306)
    //               << std::endl;
    // #endif

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