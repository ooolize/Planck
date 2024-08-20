/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#include <sched.h>

#include <iostream>

#include "timer_manager.h"
#include "utils/time.h"

int main() {
  lz::setCPUAffinity(5);
  int cpu = sched_getcpu();  // 获取当前线程所在的 CPU 核心编号
  std::cout << "Thread running on CPU: " << cpu << std::endl;

  planck::TimerManager manager;
  constexpr std::size_t ns = 1;
  constexpr std::size_t us = ns * 1000;
  constexpr std::size_t ms = us * 1000;
  constexpr std::size_t s = ms * 1000;
  auto offset = s * 1 + ms * 10 + us * 5;
  auto interval = 0 * ms + 10 * us;
  std::cout << "offset: " << offset << std::endl;
  manager.addTimer(
    offset, interval, 5, []() { std::cout << "Hello World" << std::endl; });
  manager.run();
  return 0;
}
// copy or move   perf