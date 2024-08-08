/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-09
 * @LastEditors: lize
 */
#include <chrono>
namespace lztime {

inline int rdtsc() {
  unsigned int lo, hi;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return (hi << 16) | lo;
}

inline size_t rdtsc2nanoTime(size_t rdtsc, float frequencyGHz) {
  return rdtsc / frequencyGHz;
}

inline size_t nanoTime2rdtsc(size_t nanoTime, float frequencyGHz) {
  return nanoTime * frequencyGHz;
}

inline float getFrequencyGHz() {
  return 3.7;
}

inline size_t getTimeStamp() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
           std::chrono::system_clock::now().time_since_epoch())
    .count();
}

}  // namespace lztime