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
  NanoTime getRdtscTime() const;
  ID getId() const;
  bool getStatus() const;
  ControlStgSPtr getControlStg() const;

  void OnTimer();
  void setId(ID id);
  void setStatus(bool status);

  NanoTime _rdtsc_timestamp_real_start{};

  NanoTime _rdtsc_timestamp_plan_wake{};
  NanoTime _rdtsc_timestamp_real_wake{};

  CallBack _callback{};

 private:
  ID _id{};
  //   TimeStampNs _timestamp{};
  //   NanoTime _delt_rdtsc{};
  //   NanoTime _delt_timestamp{};
  int _time_interval{};
  int _repeat{};
  //   bool _status = false;
  ControlStgSPtr _control_stg{};
  static double _frequence;
};
}  // namespace planck