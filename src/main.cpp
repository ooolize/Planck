/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#include <iostream>

#include "timer_manager.h"
#include "utils/time.h"
int main() {
  planck::TimerManager manager;
  constexpr size_t second = 1;
  constexpr size_t multi = 1000;
  auto offset = second * multi * multi;
  manager.addTimer(planck::Timer(lztime::getTimeStamp() + offset, 0, 1, []() {
    std::cout << "Hello World" << std::endl;
  }));
  manager.run();
  return 0;
}