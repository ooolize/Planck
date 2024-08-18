/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-11
 * @LastEditors: lize
 */
#pragma once
#include "interface/define.h"
namespace planck {

class ControlStg {
 public:
  virtual void strategy(Timer& current_timer) = 0;

  virtual ~ControlStg() noexcept = default;
  ControlStg(const ControlStg&) = delete;
  ControlStg& operator=(const ControlStg&) = delete;
  ControlStg(ControlStg&&) = delete;
  ControlStg& operator=(ControlStg&&) = delete;

 protected:
  ControlStg() = default;
};
}  // namespace planck