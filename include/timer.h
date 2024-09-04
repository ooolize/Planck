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
#include "utils/time.h"
namespace planck {

struct Timer {
  // friend bool operator<(const Timer& lhs, const Timer& rhs);
  // friend bool operator>(const Timer& lhs, const Timer& rhs);

  friend std::strong_ordering operator<=>(const Timer& lhs, const Timer& rhs);
  // if not default, <=> operator only generate < > <= >=. need to define ==
  friend bool operator==(const Timer& lhs, const Timer& rhs);

  friend void swap(Timer& lhs, Timer& rhs) noexcept;
  Timer() = default;
  Timer(ID id);
  Timer(TimeStampNs offset,
        int time_interval,
        int repeat,
        CallBack callback,
        ControlStgSPtr control_stg = nullptr);

  Timer(const Timer& other) = default;
  Timer(Timer&& other) noexcept;
  Timer& operator=(const Timer& other) = default;
  Timer& operator=(Timer&& other) noexcept;

  NanoTime DurationCurrToWakeup() const;
  void OnTimer();

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