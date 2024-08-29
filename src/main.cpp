/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#include <pthread.h>
#include <sched.h>

#include <atomic>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

#include "concurrentqueue.h"
#include "interface/define.h"
#include "locator/locator.h"
#include "utils/math.h"
#include "utils/system.h"
#include "utils/time.h"
void test() {
  boost::asio::io_context io_context;
  auto _callback = []() {
    std::cout << "Callback executed asynchronously!" << std::endl;
  };
  // 异步任务函数
  auto async_call = [&_callback]() -> boost::asio::awaitable<void> {
    _callback();  // 异步调用 _callback()
    co_return;    // 使用 co_return 结束协程
  };

  // 使用协程异步调用 _callback()
  boost::asio::co_spawn(io_context, async_call, boost::asio::detached);

  // 使用协程异步调用 _callback()
  boost::asio::co_spawn(io_context, async_call, boost::asio::detached);
  io_context.run();
}

int main() {
  // test
  auto& manager = planck::Locator::getTimerManager();

  test();

  // return 0;

  lz::system::setCPUAffinity(5);
  int cpu = sched_getcpu();  // 获取当前线程所在的 CPU 核心编号 3.69306
  std::cout << "Thread running on CPU: " << cpu << std::endl;

  constexpr std::size_t ns = 1;
  constexpr std::size_t us = ns * 1000;
  constexpr std::size_t ms = us * 1000;
  constexpr std::size_t s = ms * 1000;
  constexpr std::size_t count = 100;
  std::string file_name = "timer.txt";
  std::ofstream file(file_name);
  std::vector<std::size_t> v;
  v.reserve(count);
  auto offset = s * 1 + ms * 10 + us * 5;
  auto interval = 1 * ms + 0 * us;
  auto id = manager.addTimer(offset, interval, count, [&v]() {
    auto time = lz::rdtsc();
    v.push_back(time);
  });
  // auto timer = manager.getCurrentTimer();
  v.push_back(id);
  manager.start();
  std::cout << "main thread id = " << lz::system::gettid() << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));
  // std::for_each(
  //   v.begin(), v.end(), [](auto& i) { std::cout << i << std::endl; });
  // 打印每两个之间的时间差
  for (auto i = 1; i < v.size(); ++i) {
    auto time = v[i] - v[i - 1];
    file << v[i] - v[i - 1] << " " << std::fixed << time / 3.69306 << std::endl;
    // std::cout << v[i] - v[i - 1] << std::endl;
  }
  std::vector v2(v.size(), 0);
  std::adjacent_difference(v.begin(), v.end(), v2.begin());
  lz::math::calculate_statistics(v2);
  file.close();

  return 0;
}
