/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#include "timer.h"

#include <cmath>
#include <iomanip>

#include "strategy/high_speed_strategy.h"
namespace planck {

double Timer::_frequence = lz::getFrequencyGHz();

Timer::Timer(ID id) : _rdtsc_timestamp_plan_wake(id) {
}
Timer::Timer(TimeStampNs offset,
             int time_interval,
             int repeat,
             CallBack callback,
             ControlStgSPtr control_stg)
  : _repeat(repeat),
    _time_interval(time_interval),
    _callback(callback),
    _control_stg(control_stg) {
  // maybe loss 50ns because _frequence accuracy
  auto _delt_rdtsc = lz::nanoTime2rdtsc(offset, _frequence);
  _rdtsc_timestamp_real_start = lz::rdtscp();  // NOLINT
  _rdtsc_timestamp_plan_wake =                 // NOLINT
    _delt_rdtsc + _rdtsc_timestamp_real_start;

  // TODO(): must write in the inner func . why i can not write as default param
  if (!_control_stg) {
    _control_stg = std::make_shared<HighSpeedControlStg>();
  }
}

Timer::Timer(Timer&& other) noexcept {
  swap(*this, other);
}

Timer& Timer::operator=(Timer&& other) noexcept {
  swap(*this, other);
  return *this;
}

NanoTime Timer::DurationCurrToWakeup() const {
  auto curr = lz::rdtscp();
  // TODO(): narrow_cast.
  auto sleep_time =
    _rdtsc_timestamp_plan_wake > curr ? _rdtsc_timestamp_plan_wake - curr : 0;
  return std::floor(sleep_time / Timer::_frequence);  // NOLINT
}

void Timer::OnTimer() {
  // _rdtsc_timestamp_real_wake = lz::rdtscp();
  _callback();
#ifdef DEBUG
  // TODO(): narrow_cast.
  // std::cout << "real_wake" << _rdtsc_timestamp_real_wake << std::endl;
  // std::cout << "plan_wake" << _rdtsc_timestamp_plan_wake << std::endl;
  // std::cout << std::fixed << "real spend time: "
  //           << std::floor(
  //                (_rdtsc_timestamp_real_wake - _rdtsc_timestamp_real_start) /
  //                _frequence)
  //           << std::endl;  // NOLINT
  // std::cout << std::fixed << "plan spend time: "
  //           << std::floor(
  //                (_rdtsc_timestamp_plan_wake - _rdtsc_timestamp_real_start) /
  //                _frequence)
  //           << std::endl;  // NOLINT
#endif
}

std::strong_ordering operator<=>(const Timer& lhs, const Timer& rhs) {
  return lhs._rdtsc_timestamp_plan_wake <=> rhs._rdtsc_timestamp_plan_wake;
}

bool operator==(const Timer& lhs, const Timer& rhs) {
  return lhs._rdtsc_timestamp_plan_wake == rhs._rdtsc_timestamp_plan_wake;
}

void swap(Timer& lhs, Timer& rhs) noexcept {
  std::swap(lhs._repeat, rhs._repeat);
  std::swap(lhs._time_interval, rhs._time_interval);
  std::swap(lhs._callback, rhs._callback);
  std::swap(lhs._control_stg, rhs._control_stg);
  std::swap(lhs._rdtsc_timestamp_real_start, rhs._rdtsc_timestamp_real_start);
  std::swap(lhs._rdtsc_timestamp_plan_wake, rhs._rdtsc_timestamp_plan_wake);
  std::swap(lhs._rdtsc_timestamp_real_wake, rhs._rdtsc_timestamp_real_wake);
}

// inline bool operator<(const Timer& lhs, const Timer& rhs) {
//   return lhs._id < rhs._id;
// }
// inline bool operator>(const Timer& lhs, const Timer& rhs) {
//   return lhs._id > rhs._id;
// }

}  // namespace planck