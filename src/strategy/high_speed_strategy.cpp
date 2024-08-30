/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-12
 * @LastEditors: lize
 */
#include "strategy/high_speed_strategy.h"

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>

#include "locator.h"
namespace planck {
HighSpeedControlStg::HighSpeedControlStg(std::size_t before_wake_us)
  : _before_wake(before_wake_us) {
}
void HighSpeedControlStg::strategy(Timer& current_timer) {
  // std::size_t to_target_time = 0;
  // NanoTime unit_sleep_time = 0;
  boost::asio::io_context io_context;

  auto async_call = [&current_timer]() -> boost::asio::awaitable<void> {
    current_timer._callback();  // 异步调用 _callback()
    co_return;                  // 使用 co_return 结束协程
  };
  auto& is_cut = Locator::getTimerManager().getCut();
  auto& mutex = Locator::getTimerManager().getMutex();
  auto& cv = Locator::getTimerManager().getcv();
  std::size_t to_target_time = current_timer.DurationCurrToWakeup();
  NanoTime step_sleep_time = 0;
  NanoTime start = 0;
  NanoTime end = 0;
  NanoTime real_sleep_time = 0;
  while (to_target_time > _before_wake) {
    // need do some thing .otherwise os will not schedule the thread
    // lz::rdtscp();
    // 必须每次求值 不能依赖real_sleep_time
    to_target_time = current_timer.DurationCurrToWakeup();
    step_sleep_time = to_target_time - to_target_time / _convergence_coefficent;
    if (step_sleep_time < _1ms) {
      step_sleep_time -= _sleep_compensation;
    }
    start = lz::rdtscp();
    // std::this_thread::sleep_for(std::chrono::nanoseconds(step_sleep_time));
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait_for(lock, std::chrono::nanoseconds(step_sleep_time), [&is_cut]() {
      return is_cut.load();
    });
    mutex.unlock();

    // if cut in line , break
    if (is_cut.load()) {
      std::cout << "cut" << std::endl;
      return;
    }
    end = lz::rdtscp();
    real_sleep_time = lz::spendTimeNs(start, end, current_timer._frequence);
    // std::cout << " real sleep time:" << real_sleep_time
    //           << " plan sleep time:" << step_sleep_time
    //           << " remaind sleep time:" << to_target_time << std::endl;

    // if bias is too large , advance the time remaining
    if (real_sleep_time > to_target_time) {
      std::cout << " late" << std::endl;
      break;
    }

    // std::cout << "to_target_time: " << to_target_time << std::endl;
  }
  // end = lz::rdtscp();
  // std::cout << " end: " << end << std::endl;
  // Locator::getTaskPool().dispatch([]() {});
  // Locator::getTaskPool().dispatch([]() {});
  // busy wait rdstc
  std::size_t current_rdtsc = 0;
  while (current_rdtsc < current_timer._rdtsc_timestamp_plan_wake) {
    current_rdtsc = lz::rdtscp();
  };
  // end = lz::rdtscp();
  // std::cout << " end2: " << end << std::endl;
  // auto p = current_timer;

  // 同步调用
  // current_timer._callback();

  // 使用线程池的异步方案 从派发到执行的时间正常是5us左右
  // 但可能由于长时间不调用 环境变量的唤醒时间会变长为20us
  Locator::getTaskPool().dispatch(std::move(current_timer._callback));

  // 使用协程
  // boost::asio::co_spawn(io_context, async_call, boost::asio::detached);
  // io_context.run();
  return;
}
}  // namespace planck