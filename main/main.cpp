/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#include <pthread.h>
#include <sched.h>

#include <atomic>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

#include "locator.h"
#include "utils/math.h"

int main() {
  // test
  auto& manager = planck::Locator::getTimerManager();
  manager.setCPU(5);

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
  // auto offset2 = s * 1 + ms * 10 + us * 5;
  auto interval = 0 * ms + 100 * us;
  auto id1 = manager.addTimer(offset, interval, count, [&v]() {
    auto time = lz::rdtsc();
    v.push_back(time);
  });

  manager.start();
  std::this_thread::sleep_for(std::chrono::seconds(3));
  // auto id2 = manager.addTimer(offset2, interval, 1, [&v, &id1]() {
  //   auto time = lz::rdtsc();
  //   v.push_back(time);
  //   // v.push_back(id1);
  //   std::cout << "time = " << time << std::endl;
  // });
  v.push_back(id1);
  // v.push_back(id2);
  // std::this_thread::sleep_for(std::chrono::seconds(10));
  // file << lz::rdtsc2nanoTime(v[0], planck::Locator::getFrequenceGHz())
  //      << std::endl;
  // 打印每两个之间的时间差
  for (auto i = 1; i < v.size(); ++i) {
    auto time = v[i] - v[i - 1];
    file << time << " " << std::fixed << time / 3.69306 << " "
         << lz::spendTimeNs(v[0], v[i], planck::Locator::getFrequenceGHz())
         << std::endl;
  }
  std::vector v2(v.size(), 0);
  std::adjacent_difference(v.begin(), v.end(), v2.begin());
  lz::math::calculate_statistics(v2);
  file.close();

  return 0;
}
