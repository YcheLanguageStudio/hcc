// XFAIL: Linux
// RUN: %hc %s -I/opt/hsa/include -L/opt/hsa/lib -lhsa-runtime64 -o %t.out && %t.out

#include <hc.hpp>

#include <iostream>
#include <random>

#include <hsa.h>

// loop to deliberately slow down kernel execution
#define LOOP_COUNT (1024)

#define TEST_DEBUG (0)

// An example which shows how to use accelerator_view::create_marker()
///
/// The test case only works on HSA because it directly uses HSA runtime API
/// It would use completion_future::getNativeHandle() to retrieve the
/// underlying hsa_signal_t data structure to query if dependent kernels have
/// really finished execution before the new kernel is executed.
bool test() {
  bool ret = true;

  // define inputs and output
  const int vecSize = 2048;

  int table_a[vecSize];
  int table_b[vecSize];
  int table_c[vecSize];
  int *p_a = &table_a[0];
  int *p_b = &table_b[0];
  int *p_c = &table_c[0];

  // initialize test data
  std::random_device rd;
  std::uniform_int_distribution<int32_t> int_dist;
  for (int i = 0; i < vecSize; ++i) {
    table_a[i] = int_dist(rd);
    table_b[i] = int_dist(rd);
  }

  // launch kernel
  hc::extent<1> e(vecSize);
  hc::completion_future fut = hc::parallel_for_each(
    e,
    [=](hc::index<1> idx) restrict(amp) {
      for (int i = 0; i < LOOP_COUNT; ++i) 
        p_c[idx[0]] = p_a[idx[0]] + p_b[idx[0]];

  });

  // create a barrier packet
  hc::accelerator_view av = hc::accelerator().get_default_view();
  hc::completion_future fut2 = av.create_marker();

  void* nativeHandle = fut.getNativeHandle();
  void* nativeHandle2 = fut2.getNativeHandle();

#if TEST_DEBUG
  std::cout << nativeHandle << "\n";
  std::cout << nativeHandle2 << "\n";
#endif

  hsa_signal_value_t signal_value;
  hsa_signal_value_t signal_value2;

  signal_value = hsa_signal_load_relaxed(*static_cast<hsa_signal_t*>(nativeHandle));
#if TEST_DEBUG
  std::cout << "kernel signal value: " << signal_value << "\n";
#endif
  ret &= (signal_value == 1);

  signal_value2 = hsa_signal_load_relaxed(*static_cast<hsa_signal_t*>(nativeHandle2));
#if TEST_DEBUG
  std::cout << "barrier signal value: " << signal_value << "\n";
#endif
  ret &= (signal_value2 == 1);

  // wait on the barrier packet
  fut2.wait();

  // the barrier packet would ensure all previous packets were processed

  signal_value = hsa_signal_load_relaxed(*static_cast<hsa_signal_t*>(nativeHandle));
#if TEST_DEBUG
  std::cout << "kernel signal value: " << signal_value << "\n";
#endif
  ret &= (signal_value == 0);

  signal_value2 = hsa_signal_load_relaxed(*static_cast<hsa_signal_t*>(nativeHandle2));
#if TEST_DEBUG
  std::cout << "barrier signal value: " << signal_value << "\n";
#endif
  ret &= (signal_value2 == 0);

  // verify
  int error = 0;
  for(unsigned i = 0; i < vecSize; i++) {
    error += table_c[i] - (table_a[i] + table_b[i]);
  }
  if (error == 0) {
    std::cout << "Verify success!\n";
  } else {
    std::cout << "Verify failed!\n";
  }
  ret &= (error == 0);

  return ret;
}

int main() {
  bool ret = true;

  ret &= test();

  return !(ret == true);
}


