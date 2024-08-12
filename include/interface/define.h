/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-12
 * @LastEditors: lize
 */
#pragma once
#include <memory>

#include "utils/time.h"
namespace planck {
class Timer;
class ControlStg;
static double frequence = lz::getFrequencyGHz();
static std::size_t timer_id = 0;

using ID = std::size_t;
using NanoTime = std::uint64_t;
using GHz = float;
using TimeStampNs = std::uint64_t;
using CallBack = void (*)();
using TimerSPtr = std::shared_ptr<Timer>;
using ControlStgSPtr = std::shared_ptr<ControlStg>;
using ControlStgUPtr = std::unique_ptr<ControlStg>;
}  // namespace planck
