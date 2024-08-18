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

void test() {
  auto start = lz::rdtscp();
  auto end = lz::rdtscp();
  std::size_t count = 1000 * 1000 * 1000;
  int i = 10;
  std::size_t sum = 0;
  auto myfunc = []() { std::cout << "Hello World" << std::endl; };

  while (i--) {
    start = lz::rdtscp();
    planck::Timer timer(count, 0, 1, myfunc);
    // std::size_t targetRtsc = lz::rdtscp() + count;
    // timer.getControlStg()->strategy(timer);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 10));
    // std::size_t current_rdtsc = 0;
    // timer.getControlStg()->strategy(timer);
    end = lz::rdtscp();
    auto delta = end - start;
    sum += delta;
    std::cout << i << " end rdtsc " << end << " "
              << lz::spendTimeNs(0, delta, 3.69306) << std::endl;
  }
  std::cout << "average cost: " << lz::spendTimeNs(0, sum / 10, 3.69306)
            << std::endl;
}

int main() {
  lz::setCPUAffinity(5);
  int cpu = sched_getcpu();  // 获取当前线程所在的 CPU 核心编号
  std::cout << "Thread running on CPU: " << cpu << std::endl;
  // test();
  planck::TimerManager manager;
  constexpr std::size_t ns = 1;
  constexpr std::size_t us = ns * 1000;
  constexpr std::size_t ms = us * 1000;
  constexpr std::size_t s = ms * 1000;
  auto offset = s * 1 + ms * 10 + us * 5;
  std::size_t count = 1000 * 1000;
  std::cout << "offset: " << offset << std::endl;
  auto func = []() { std::cout << "Hello World" << std::endl; };
  // auto temp = lz::getTimeStampNs();
  // temp = temp + offset;
  auto start = lz::rdtscp();
  planck::Timer timer(offset, 0, 1, func);
  auto af_create = lz::rdtscp();
  // std::this_thread::sleep_for(std::chrono::seconds(1));
  manager.addTimer(std::move(timer));
  auto af_add = lz::rdtscp();
  manager.run();
  auto end = lz::rdtscp();

  std::cout << "create timer cost: "
            << lz::spendTimeNs(start, af_create, 3.69306) << std::endl;

  std::cout << "add timer cost: " << lz::spendTimeNs(af_create, af_add, 3.69306)
            << std::endl;

  std::cout << "run timer cost: " << lz::spendTimeNs(af_add, end, 3.69306)
            << std::endl;
  ;
  std::cout << "total timer cost: " << lz::spendTimeNs(start, end, 3.69306)
            << std::endl;

  return 0;
}
// copy or move   perf