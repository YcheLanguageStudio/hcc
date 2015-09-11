// XFAIL: Linux
// RUN: %hc %s -o %t.out && %t.out

#include <hc.hpp>

#include <iostream>

// a test which checks if hc::get_system_ticks() works
bool test() {

  hc::accelerator acc;
  hc::accelerator_view av = acc.get_default_view();

  bool ret = true;

  // check if the queue is HSA
  ret &= av.get_hsa_interop();

  std::cout << ret << "\n";

  // use get_system_ticks()
  uint64_t tick1 = hc::get_system_ticks();

  // sleep for 1 second
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // use get_system_ticks()
  uint64_t tick2 = hc::get_system_ticks();

  ret &= (tick2 > tick1);

  std::cout << ret << "\n";

  return ret;
}

int main() {
  bool ret = true;

  ret &= test();

  return !(ret == true);
}

