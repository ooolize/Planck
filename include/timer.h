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
  Timer(TimeStampNs offset,
        int time_interval,
        int repeat,
        CallBack callback,
        ControlStgSPtr control_stg = nullptr);

  NanoTime getSleepTime() const;
  void OnTimer();

 public:
  ID _id{};
  int _repeat{};
  int _time_interval{};
  CallBack _callback{};
  ControlStgSPtr _control_stg{};

  NanoTime _rdtsc_timestamp_real_start{};
  NanoTime _rdtsc_timestamp_plan_wake{};
  NanoTime _rdtsc_timestamp_real_wake{};

  static double _frequence;
};
}  // namespace planck