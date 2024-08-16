/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#include "timer.h"

#include <cmath>
#include <iomanip>
namespace planck {

double Timer::_frequence = lz::getFrequencyGHz();
Timer::Timer(ID id) : _id(id) {
}

Timer::Timer(TimeStampNs timestamp,
             int time_interval,
             int repeat,
             CallBack callback,
             ControlStgSPtr control_stg)
  : _timestamp(timestamp),
    _time_interval(time_interval),
    _repeat(repeat),
    _status(true),
    _callback(callback),
    _control_stg(std::move(control_stg)) {
#ifdef DEBUG
  auto tmp1 = lz::rdtscp();
#endif
  // TODO(): must write in the inner func . why i can not write as default param
  if (!_control_stg) {
    _control_stg = std::make_shared<HighSpeedControlStg>();
  }
  auto delt = timestamp - lz::getTimeStampNs();
  _rdtsc_timestamp_real_start = lz::rdtscp();
  _rdtsc_timestamp_plan_wake =
    lz::nanoTime2rdtsc(delt, _frequence) + _rdtsc_timestamp_real_start;
#ifdef DEBUG
  std::cout << "create timer cost "
            << lz::spendTimeNs(tmp1, lz::rdtscp(), 3.69306) << std::endl;
#endif
}

NanoTime Timer::getSleepTime() const {
#ifdef DEBUG
  // auto tmp = lz::rdtscp();
  // auto delta_rdtsc = (tmp - _rdtsc_timestamp_plan_wake);
  // auto delta_ns = delta_rdtsc / _frequence;
  // double delta_floor_ns = std::floor(delta_ns);
  // NanoTime sleep_time = delta_floor_ns;
  // std::cout << "current rdtsc: " << tmp << std::endl;
  // std::cout << "set rdtsc: " << _rdtsc_timestamp_plan_wake << std::endl;
  // std::cout << "_frequence: " << _frequence << std::endl;
  // std::cout << "delta_rdtsc: " << delta_rdtsc << std::endl;
  // std::cout << "delta_ns: " << delta_ns << std::endl;
  // std::cout << "delta_floor_ns: " << delta_floor_ns << std::endl;
  // std::cout << "sleep_time: " << sleep_time << std::endl;
#endif
  // TODO(): narrow_cast.
  return std::floor((_rdtsc_timestamp_plan_wake - lz::rdtscp()) /
                    _frequence);  // NOLINT
}
NanoTime Timer::getRdtscTime() const {
  return _rdtsc_timestamp_plan_wake;
}
void Timer::OnTimer() {
  _rdtsc_timestamp_real_wake = lz::rdtscp();
  _callback();
  // #ifdef DEBUG
  // TODO(): narrow_cast.

  std::cout << std::fixed << "real spend time: "
            << std::floor(
                 (_rdtsc_timestamp_real_wake - _rdtsc_timestamp_real_start) /
                 _frequence)
            << std::endl;  // NOLINT
  // #endif
}
ID Timer::getId() const {
  return _id;
}
void Timer::setId(ID id) {
  _id = id;
}
bool Timer::getStatus() const {
  return _status;
}
void Timer::setStatus(bool status) {
  _status = status;
}
ControlStgSPtr Timer::getControlStg() const {
  return _control_stg;
}

std::strong_ordering operator<=>(const Timer& lhs, const Timer& rhs) {
  return lhs._id <=> rhs._id;
}

bool operator==(const Timer& lhs, const Timer& rhs) {
  return lhs._id == rhs._id;
}
// inline bool operator<(const Timer& lhs, const Timer& rhs) {
//   return lhs._id < rhs._id;
// }
// inline bool operator>(const Timer& lhs, const Timer& rhs) {
//   return lhs._id > rhs._id;
// }

}  // namespace planck