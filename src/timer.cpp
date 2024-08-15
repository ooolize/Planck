/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#include "timer.h"

#include <cmath>
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
  // TODO(): must write in the inner func . why i can not write as default param
  if (!_control_stg) {
    _control_stg = std::make_shared<HighSpeedControlStg>();
  }
  auto delt = timestamp - lz::getTimeStampNs();
  _rdtsc_timestamp = lz::nanoTime2rdtsc(delt, _frequence) + lz::rdtsc();
#ifdef DEBUG
  std::cout << "delt ns: " << delt << std::endl;
  std::cout << "set rdtsc time: " << _rdtsc_timestamp << std::endl;
#endif
}

NanoTime Timer::getSleepTime() const {
#ifdef DEBUG
  std::cout << "current rdtsc: " << lz::rdtsc() << std::endl;
  std::cout << "set rdtsc: " << _rdtsc_timestamp << std::endl;
  std::cout << "_frequence: " << _frequence << std::endl;
#endif
  // TODO(): narrow_cast.
  return std::floor((lz::rdtsc() - _rdtsc_timestamp) / _frequence);  // NOLINT
}
NanoTime Timer::getRdtscTime() const {
  return _rdtsc_timestamp;
}
void Timer::OnTimer() {
  _callback();
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