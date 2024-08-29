/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-28
 * @LastEditors: lize
 */
#include "locator/locator.h"
namespace planck {

TaskPool Locator::_thread_pool;
TimerManager Locator::_timer_manager;
}  // namespace planck