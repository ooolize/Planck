/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-28
 * @LastEditors: lize
 */
#include "locator.h"

#include "utils/time.h"
namespace planck {

TaskPool Locator::_thread_pool;
TimerManager Locator::_timer_manager;
double Locator::_frequence = lz::getFrequencyGHz();
}  // namespace planck