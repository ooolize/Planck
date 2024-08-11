/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-09
 * @LastEditors: lize
 */
#pragma once
#include <chrono>
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
  return 3.7;
}

inline std::size_t getTimeStamp() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
           std::chrono::system_clock::now().time_since_epoch())
    .count();
}

}  // namespace lz