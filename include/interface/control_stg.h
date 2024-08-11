/*
 * @Description:
 * @Author: lize
 * @Date: 2024-08-11
 * @LastEditors: lize
 */
#pragma once
namespace planck {
class ControlStg {
 public:
  virtual void strategy() = 0;

  virtual ~ControlStg() noexcept = 0;
  ControlStg(const ControlStg&) = delete;
  ControlStg& operator=(const ControlStg&) = delete;
  ControlStg(ControlStg&&) = delete;
  ControlStg& operator=(ControlStg&&) = delete;

 protected:
  ControlStg() = default;
};
}  // namespace planck