/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#pragma once
#include <compare>
#include <memory>

#include "interface/define.h"
#include "strategy/high_speed_strategy.h"
#include "utils/time.h"
namespace planck {

class Timer {
 public:
  // friend bool operator<(const Timer& lhs, const Timer& rhs);
  // friend bool operator>(const Timer& lhs, const Timer& rhs);

  friend std::strong_ordering operator<=>(const Timer& lhs, const Timer& rhs);
  // if not default, <=> operator only generate < > <= >=. need to define ==
  friend bool operator==(const Timer& lhs, const Timer& rhs);

  explicit Timer(ID id);
  Timer() = default;
  Timer(TimeStampNs timestamp,
        int time_interval,
        int repeat,
        CallBack callback,
        ControlStgSPtr control_stg = nullptr);

  NanoTime getSleepTime() const;
  NanoTime getRdtscTime() const;
  ID getId() const;
  bool getStatus() const;
  ControlStgSPtr getControlStg() const;

  void OnTimer();
  void setId(ID id);
  void setStatus(bool status);

 private:
  ID _id{};
  NanoTime _rdtsc_timestamp{};
  TimeStampNs _timestamp{};
  int _time_interval{};
  int _repeat{};
  bool _status = false;
  CallBack _callback{};
  ControlStgSPtr _control_stg{};
  static double _frequence;
};
}  // namespace planck