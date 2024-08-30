<!--
 * @Description: 
 * @Author: lize
 * @Date: 2024-08-19
 * @LastEditors: lize
-->

# Planck

<p align="center">
  <a>
  <img src="asset//ruler.svg" alt="Planck" width="100" height="100">
  </a>
</p>




## Introduce

一个高性能定时器

## Feature

+ 10us级别精度
+ 自定义每个timer唤醒策略


## Use

```c++
auto &manager = planck::Locator::getTimerManager();
manager.setCPU(5);
manager.start();

auto func = [](){ std::cout << "Planck " << std::endl; };
auto id1 = manager.addTimer(offset, interval, count, func);

```