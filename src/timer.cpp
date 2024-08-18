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

Timer::Timer(TimeStampNs offset,
             int time_interval,
             int repeat,
             CallBack callback,
             ControlStgSPtr control_stg)
  : _time_interval(time_interval),
    _repeat(repeat),
    _callback(callback),
    _control_stg(std::move(control_stg)) {
#ifdef DEBUG
  // auto tmp1 = lz::rdtscp();
#endif
  // TODO(): must write in the inner func . why i can not write as default param
  // auto curr_timestamp = lz::getTimeStampNs();
  // auto _delt_timestamp = timestamp - curr_timestamp;

  auto _delt_rdtsc = lz::nanoTime2rdtsc(offset, 3.69306);
  // std::cout << "delt timestamp: " << _delt_timestamp << std::endl;
  // std::cout << "delt rdtsc: " << _delt_rdtsc << std::endl;
  // _delt_rdtsc = delta_rdtsc;
  _rdtsc_timestamp_real_start = lz::rdtscp();
  _rdtsc_timestamp_plan_wake = _delt_rdtsc + _rdtsc_timestamp_real_start;

  if (!_control_stg) {
    _control_stg = std::make_shared<HighSpeedControlStg>();
  }

#ifdef DEBUG
  // auto end = lz::rdtscp();
  // std::cout << "create timer cost " << lz::spendTimeNs(tmp1, end, 3.69306)
  //           << std::endl;
#endif
}

NanoTime Timer::getSleepTime() const {
  auto curr = lz::rdtscp();
#ifdef DEBUG
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
  return std::floor((_rdtsc_timestamp_plan_wake - curr) / 3.69306);  // NOLINT
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
  std::cout << std::fixed << "plan spend time: "
            << std::floor(
                 (_rdtsc_timestamp_plan_wake - _rdtsc_timestamp_real_start) /
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