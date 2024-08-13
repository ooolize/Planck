/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-09
 * @LastEditors: lize
 */
#pragma once
#include <chrono>
#include <fstream>
namespace lz {

inline uint64_t rdtsc() {
  uint32_t lo, hi;
  // Use CPUID to serialize instruction execution
  __asm__ volatile("cpuid" : : : "%rax", "%rbx", "%rcx", "%rdx");
  // Execute RDTSC instruction
  __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
  return (uint64_t)hi << 32 | lo;
}

inline std::size_t rdtsc2nanoTime(std::size_t rdtsc, float frequencyGHz) {
  return rdtsc / frequencyGHz;
}

inline std::size_t nanoTime2rdtsc(std::size_t nanoTime, float frequencyGHz) {
  return nanoTime * frequencyGHz;
}

inline float getFrequencyGHz() {
  std::ifstream cpuinfo("/proc/cpuinfo");
  std::string line;
  std::string target = "cpu MHz";

  if (cpuinfo.is_open()) {
    while (std::getline(cpuinfo, line)) {
      if (line.find(target) != std::string::npos) {
        // 找到包含 "cpu MHz" 的行
        std::istringstream iss(line);
        std::string label;
        double mhz;
        iss >> label >> label >> mhz;  // 读取 "cpu MHz" 和数值

        std::cout << "CPU MHz: " << mhz << std::endl;
        return mhz / 1000;
      }
    }
    cpuinfo.close();
  } else {
    std::cerr << "无法打开 /proc/cpuinfo 文件" << std::endl;
  }
  return 0;
  // return 3.7;
}

inline std::size_t getTimeStampNs() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
           std::chrono::system_clock::now().time_since_epoch())
    .count();
}

}  // namespace lz