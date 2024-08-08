/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#pragma once
#include "utils/time.h"
namespace planck {

using NanoTime = unsigned long;
using GHz = float;
using TimeStamp = unsigned long;
using size_t = unsigned long;
using CallBack = void (*)();
class Timer {
 public:
  Timer() = default;
  Timer(TimeStamp timestamp, int time_interval, int repeat, CallBack callback)
    : _timestamp(timestamp),
      _time_interval(time_interval),
      _repeat(repeat),
      _status(true),
      _callback(callback) {
    auto delat = timestamp - lztime::getTimeStamp();
    _rdtsc_time = lztime::rdtsc() +
                  lztime::nanoTime2rdtsc(delat, lztime::getFrequencyGHz());
  }
  NanoTime getRdtscTime() const {
    return _rdtsc_time;
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
  NanoTime _rdtsc_time{};
  TimeStamp _timestamp{};
  int _time_interval{};
  int _repeat{};
  bool _status = false;
  CallBack _callback;
};

}  // namespace planck