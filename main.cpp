/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-08
 * @LastEditors: lize
 */
#include <iostream>

#include "manager.h"

int main() {
  planck::TimerManager manager;
  manager.addTimer(
    planck::Timer(0, 0, 1, []() { std::cout << "Hello World" << std::endl; }));
  manager.run();
  return 0;
}