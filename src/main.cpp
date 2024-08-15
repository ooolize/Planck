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
  int cpu = sched_getcpu();  // 获取当前线程所在的 CPU 核心编号
  std::cout << "Thread running on CPU: " << cpu << std::endl;
  planck::TimerManager manager;
  constexpr std::size_t second = 1;
  constexpr std::size_t multi = 1000;
  auto offset = second * multi * multi * multi;
  auto func = []() { std::cout << "Hello World" << std::endl; };
  manager.addTimer(planck::Timer(lz::getTimeStampNs() + offset, 0, 1, func));
  manager.run();
  return 0;
}