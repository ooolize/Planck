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
  constexpr std::size_t second = 1;
  constexpr std::size_t multi = 1000;
  auto offset = second * multi * multi * multi;
  auto func = []() { std::cout << "Hello World" << std::endl; };
  manager.addTimer(planck::Timer(lz::getTimeStamp() + offset, 0, 1, func));
  manager.run();
  return 0;
}