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

void test1RDTSC() {
  int current_cpu = sched_getcpu();  // 获取当前 CPU 核心编号
  std::cout << "Thread started on CPU: " << current_cpu << std::endl;

  // 绑定线程到创建时的 CPU
  lz::setCPUAffinity(current_cpu);
  auto start = lz::rdtsc();
  std::cout << "start: " << start << std::endl;
  std::this_thread::sleep_for(std::chrono::nanoseconds(1000 * 1000));
  auto end = lz::rdtsc();
  std::cout << "end: " << end << std::endl;
  std::cout << " test1RDTSC spend time: "
            << lz::spendTimeNs(start, end, 3.69306) << std::endl;
}

void test2RDTSC() {
  // int current_cpu = sched_getcpu();  // 获取当前 CPU 核心编号
  // std::cout << "Thread started on CPU: " << current_cpu << std::endl;

  // 绑定线程到创建时的 CPU
  lz::setCPUAffinity(5);
  auto start = lz::rdtsc();
  auto wake_up = start + lz::nanoTime2rdtsc(1000 * 1000, 3.69306);
  while (lz::rdtsc() < wake_up);
  auto end = lz::rdtsc();
  std::cout << "test2RDTSC time spend: " << lz::spendTimeNs(start, end, 3.69306)
            << std::endl;
}
int main() {
  // test1RDTSC();
  // test2RDTSC();

  int cpu = sched_getcpu();  // 获取当前线程所在的 CPU 核心编号
  std::cout << "Thread running on CPU: " << cpu << std::endl;
  planck::TimerManager manager;
  constexpr std::size_t second = 1;
  constexpr std::size_t multi = 1000;
  auto offset = second * multi * multi;
  auto func = []() { std::cout << "Hello World" << std::endl; };
  manager.addTimer(planck::Timer(lz::getTimeStampNs() + offset, 0, 1, func));
  manager.run();
  return 0;
}