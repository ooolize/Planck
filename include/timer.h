/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#pragma once
#include <memory>

#include "utils/time.h"
namespace planck {
class Timer;

using NanoTime = unsigned long;
using GHz = float;
using TimeStamp = unsigned long;
using CallBack = void (*)();
using TimerSPtr = std::shared_ptr<Timer>;

class Timer {
 public:
  Timer() = default;
  Timer(TimeStamp timestamp, int time_interval, int repeat, CallBack callback)
    : _timestamp(timestamp),
      _time_interval(time_interval),
      _repeat(repeat),
      _status(true),
      _callback(callback) {
    auto delt = timestamp - lz::getTimeStamp();
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
    return id;
  }
  void setId(int id) {
    this->id = id;
  }
  bool getStatus() const {
    return _status;
  }
  void setStatus(bool status) {
    _status = status;
  }

 private:
  int id{};
  NanoTime _rdtsc_timestamp{};
  TimeStamp _timestamp{};
  int _time_interval{};
  int _repeat{};
  bool _status = false;
  CallBack _callback;
};

}  // namespace planck