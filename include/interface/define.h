/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-12
 * @LastEditors: lize
 */
#pragma once
#include <functional>
#include <memory>

#include "utils/time.h"
namespace planck {
class Timer;
class ControlStg;
static std::size_t timer_id = 0;

using size_t = std::size_t;
using ID = std::size_t;
using NanoTime = std::uint64_t;
using GHz = float;
using TimeStampNs = std::uint64_t;
using CallBack = std::function<void()>;
using TimerSPtr = std::shared_ptr<Timer>;
using ControlStgSPtr = std::shared_ptr<ControlStg>;
using ControlStgUPtr = std::unique_ptr<ControlStg>;
}  // namespace planck
