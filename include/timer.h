/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#pragma once
#include <compare>
#include <memory>

#include "utils/time.h"
namespace planck {
static float frequence = lz::getFrequencyGHz();
static std::size_t timer_id = 0;

class Timer;

using NanoTime = unsigned long;
using GHz = float;
using TimeStamp = unsigned long;
using CallBack = void (*)();
using TimerSPtr = std::shared_ptr<Timer>;

class Timer {
 public:
  // friend bool operator<(const Timer& lhs, const Timer& rhs);
  // friend bool operator>(const Timer& lhs, const Timer& rhs);
  friend std::strong_ordering operator<=>(const Timer& lhs, const Timer& rhs);
  // if not default, <=> operator only generate < > <= >=. need to define ==
  friend bool operator==(const Timer& lhs, const Timer& rhs);

  Timer() = default;
  Timer(TimeStamp timestamp, int time_interval, int repeat, CallBack callback)
    : _timestamp(timestamp),
      _time_interval(time_interval),
      _repeat(repeat),
      _status(true),
      _callback(callback) {
    auto delt = timestamp - lz::getTimeStamp();
    _id = timer_id++;
    _rdtsc_timestamp =
      lz::nanoTime2rdtsc(delt, lz::getFrequencyGHz()) + lz::rdtsc();
  }
  NanoTime getSleepTime() const {
    return (lz::rdtsc() - _rdtsc_timestamp) / frequence;
  }
  NanoTime getRdtscTime() const {
    return _rdtsc_timestamp;
  }
  void OnTimer() {
    _callback();
  }
  int getId() const {
    return _id;
  }
  void setId(int id) {
    _id = id;
  }
  bool getStatus() const {
    return _status;
  }
  void setStatus(bool status) {
    _status = status;
  }

 private:
  int _id{};
  NanoTime _rdtsc_timestamp{};
  TimeStamp _timestamp{};
  int _time_interval{};
  int _repeat{};
  bool _status = false;
  CallBack _callback;
};
inline std::strong_ordering operator<=>(const Timer& lhs, const Timer& rhs) {
  return lhs._id <=> rhs._id;
}

// inline bool operator<(const Timer& lhs, const Timer& rhs) {
//   return lhs._id < rhs._id;
// }
// inline bool operator>(const Timer& lhs, const Timer& rhs) {
//   return lhs._id > rhs._id;
// }

inline bool operator==(const Timer& lhs, const Timer& rhs) {
  return lhs._id == rhs._id;
}
}  // namespace planck