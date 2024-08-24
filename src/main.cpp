/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#include <pthread.h>
#include <sched.h>

#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#include "timer_manager.h"
#include "utils/time.h"

void set_real_time_priority(pthread_t thread) {
  struct sched_param param;
  param.sched_priority = sched_get_priority_max(SCHED_FIFO);
  pthread_setschedparam(thread, SCHED_FIFO, &param);
}

int main() {
  // 在线程中调用该函数以设置实时优先级
  set_real_time_priority(pthread_self());
  lz::setCPUAffinity(5);
  int cpu = sched_getcpu();  // 获取当前线程所在的 CPU 核心编号 3.69306
  std::cout << "Thread running on CPU: " << cpu << std::endl;

  planck::TimerManager manager;
  constexpr std::size_t ns = 1;
  constexpr std::size_t us = ns * 1000;
  constexpr std::size_t ms = us * 1000;
  constexpr std::size_t s = ms * 1000;
  constexpr std::size_t count = 1000;
  std::string file_name = "timer.txt";
  std::ofstream file(file_name);
  std::vector<std::size_t> v;
  v.reserve(count);
  auto offset = s * 1 + ms * 10 + us * 5;
  auto interval = 1 * ms + 0 * us;
  manager.addTimer(offset, interval, count, [&v]() {
    auto time = lz::rdtsc();
    v.push_back(time);
  });
  auto timer = manager.getCurrentTimer();
  v.push_back(timer._rdtsc_timestamp_plan_wake);
  manager.run();
  // std::for_each(
  //   v.begin(), v.end(), [](auto& i) { std::cout << i << std::endl; });
  // 打印每两个之间的时间差
  for (auto i = 1; i < v.size(); ++i) {
    file << v[i] - v[i - 1] << std::endl;
    // std::cout << v[i] - v[i - 1] << std::endl;
  }
  file.close();
  return 0;
}
// copy or move   perf