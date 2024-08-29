/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-29
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

#include "concurrentqueue.h"
#include "interface/define.h"
#include "locator/locator.h"
#include "utils/system.h"
#include "utils/time.h"
void test_cv() {
  moodycamel::ConcurrentQueue<std::size_t> _tasks{};

  std::vector<std::size_t> jv, v;
  std::condition_variable cv;
  std::mutex m;

  std::atomic<std::int32_t> count = 10;
  bool ready = false;
  jv.reserve(count);
  v.reserve(count);
  lz::system::setCPUAffinity(5);
  std::jthread t([&_tasks, &count, &ready, &cv, &m, &jv]() {
    lz::system::setCPUAffinity(5);
    std::size_t result = 0;
    while (1) {
      std::unique_lock<std::mutex> lock(m);
      cv.wait(lock, [&ready] { return ready; });
      if (_tasks.try_dequeue(result)) {
      };
      auto rdtsc = lz::rdtsc();
      ready = false;
      if (count == 0) {
        break;
      }
      lock.unlock();
      jv.push_back(rdtsc);
      // std::cout << " ready = " << ready << std::endl;
    }
  });
  std::this_thread::sleep_for(std::chrono::seconds(1));
  while (count) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    {
      std::lock_guard<std::mutex> lock(m);
      --count;
      ready = true;
    }
    auto rdtsc = lz::rdtsc();
    _tasks.enqueue(rdtsc);
    cv.notify_one();
    v.push_back(rdtsc);
  }

  for (int i = 0; i < jv.size(); ++i) {
    std::cout << jv[i] - v[i] << std::endl;
  }
}
