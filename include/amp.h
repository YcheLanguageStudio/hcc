#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <future>

/* COMPATIBILITY LAYER */
#define STD__FUTURE_STATUS__FUTURE_STATUS std::future_status

#ifndef WIN32
#define __declspec(ignored) /* */
#endif

/*
  This is not part of C++AMP standard, but borrowed from Parallel Patterns
  Library. 
*/
namespace concurrency {
  template <typename _Type> class task;
  template <> class task<void>;
}

namespace Concurrency {

enum queuing_mode {
  queuing_mode_immediate,
  queuing_mode_automatic
};

class accelerator_view;
class accelerator;

class accelerator {
public:
  static const wchar_t default_accelerator[];   // = L"default"
  static const wchar_t direct3d_warp[];         // = L"direct3d\\warp"
  static const wchar_t direct3d_ref[];          // = L"direct3d\\ref"
  static const wchar_t cpu_accelerator[];       // = L"cpu"
  
  accelerator();
  explicit accelerator(const std::wstring& path);
  accelerator(const accelerator& other);
  static std::vector<accelerator> get_all();
  static bool set_default(const std::wstring& path);
  accelerator& operator=(const accelerator& other);
  __declspec(property(get)) std::wstring device_path;
  __declspec(property(get)) unsigned int version; // hiword=major, loword=minor
  __declspec(property(get)) std::wstring description;
  __declspec(property(get)) bool is_debug;
  __declspec(property(get)) bool is_emulated;
  __declspec(property(get)) bool has_display;
  __declspec(property(get)) bool supports_double_precision;
  __declspec(property(get)) bool supports_limited_double_precision;
  __declspec(property(get)) size_t dedicated_memory;
  accelerator_view get_default_view() const;
  // __declspec(property(get=get_default_view)) accelerator_view default_view;

  accelerator_view create_view();
  accelerator_view create_view(queuing_mode qmode);
  
  bool operator==(const accelerator& other) const;
  bool operator!=(const accelerator& other) const;
};

class completion_future;
class accelerator_view {
public:
  accelerator_view() = delete;
  accelerator_view(const accelerator_view& other);
  accelerator_view& operator=(const accelerator_view& other);
  accelerator get_accelerator() const;
  // __declspec(property(get=get_accelerator)) Concurrency::accelerator accelerator;

  __declspec(property(get)) bool is_debug;
  __declspec(property(get)) unsigned int version;
  __declspec(property(get)) queuing_mode queuing_mode;
  void flush();
  void wait();
  completion_future create_marker();
  bool operator==(const accelerator_view& other) const;
  bool operator!=(const accelerator_view& other) const;
};

class completion_future {
public:
  completion_future();
  completion_future(const completion_future& _Other);
  completion_future(completion_future&& _Other);
  ~completion_future();
  completion_future& operator=(const completion_future& _Other);
  completion_future& operator=(completion_future&& _Other);

  void get() const;
  bool valid() const;
  void wait() const;

  template <class _Rep, class _Period>
  STD__FUTURE_STATUS__FUTURE_STATUS wait_for(
    const std::chrono::duration<_Rep, _Period>& _Rel_time) const;

  template <class _Clock, class _Duration>
  STD__FUTURE_STATUS__FUTURE_STATUS wait_until(
    const std::chrono::time_point<_Clock, _Duration>& _Abs_time) const;

  operator std::shared_future<void>() const;

  template <typename _Functor>
  void then(const _Functor &_Func) const;

  concurrency::task<void> to_task() const;
};

template <int N>
class index {
public:
  static const int rank = N;

  typedef int value_type;

  index() restrict(amp,cpu);

  index(const index& other) restrict(amp,cpu);

  explicit index(int i0) restrict(amp,cpu); // N==1

  index(int i0, int i1) restrict(amp,cpu); // N==2

  index(int i0, int i1, int i2) restrict(amp,cpu); // N==3

  explicit index(const int components[]) restrict(amp,cpu);

  index& operator=(const index& other) restrict(amp,cpu);

  int operator[](unsigned int c) const restrict(amp,cpu);

  int& operator[](unsigned int c) restrict(amp,cpu);
  
#if 0
  template   <int N>
  friend     bool operator==(const index<N>& lhs, const index<N>& rhs) restrict(amp,cpu);
  template   <int N>
  friend     bool operator!=(const index<N>& lhs, const index<N>& rhs) restrict(amp,cpu);
  template   <int N>
  friend     index<N> operator+(const index<N>& lhs, const index<N>& rhs) restrict(amp,cpu);
  template <int N>
  friend     index<N> operator-(const index<N>& lhs, const index<N>& rhs) restrict(amp,cpu);
#endif

  index& operator+=(const index& rhs) restrict(amp,cpu);
  index& operator-=(const index& rhs) restrict(amp,cpu);

#if 0
  template <int N>
  friend   index<N>
  operator+(const index<N>& lhs, int rhs) restrict(amp,cpu);
  template <int N>
  friend   index<N>
  operator+(int lhs, const index<N>& rhs) restrict(amp,cpu);
  template <int N>
  friend   index<N>
  operator-(const index<N>& lhs, int rhs) restrict(amp,cpu);
  template <int N>
  friend   index<N>
  operator-(int lhs, const index<N>& rhs) restrict(amp,cpu);
  template <int N>
  friend   index<N>
  operator*(const index<N>& lhs, int rhs) restrict(amp,cpu);
  template <int N>
  friend   index<N>
  operator*(int lhs, const index<N>& rhs) restrict(amp,cpu);
  template <int N>
  friend   index<N>
  operator/(const index<N>& lhs, int rhs) restrict(amp,cpu);
  template <int N>
  friend   index<N>
  operator/(int lhs, const index<N>& rhs) restrict(amp,cpu);
  template <int N>
  friend   index<N>
  operator%(const index<N>& lhs, int rhs) restrict(amp,cpu);
  template <int N>
  friend   index<N> 
  operator%(int lhs, const index<N>& rhs) restrict(amp,cpu);
#endif
  
  index& operator+=(int rhs) restrict(amp,cpu);
  index& operator-=(int rhs) restrict(amp,cpu);
  index& operator*=(int rhs) restrict(amp,cpu);
  index& operator/=(int rhs) restrict(amp,cpu);
  index& operator%=(int rhs) restrict(amp,cpu);
  
  index& operator++() restrict(amp,cpu);
  index operator++(int) restrict(amp,cpu);
  index& operator--() restrict(amp,cpu);
  index operator--(int) restrict(amp,cpu);
};

// forward decls
template <int D0, int D1=0, int D2=0> class tiled_extent;

template <typename T, int N> class array_view;

template <int N>
class extent {
public:
  static const int rank = N;

  typedef int value_type;

  extent() restrict(amp,cpu);

  extent(const extent& other) restrict(amp,cpu);

  explicit extent(int e0) restrict(amp,cpu); // N==1

  extent(int e0, int e1) restrict(amp,cpu); // N==2

  extent(int e0, int e1, int e2) restrict(amp,cpu); // N==3

  explicit extent(const int components[]) restrict(amp,cpu);

  extent& operator=(const extent& other) restrict(amp,cpu);

  int operator[](unsigned int c) const restrict(amp,cpu);

  int& operator[](unsigned int c) restrict(amp,cpu);

  int size() const restrict(amp,cpu);

  bool contains(const index<N>& idx) const restrict(amp,cpu);

  template <int D0> tiled_extent<D0> tile() const;
  template <int D0, int D1> tiled_extent<D0,D1> tile() const;
  template <int D0, int D1, int D2> tiled_extent<D0,D1,D2> tile() const;

  extent operator+(const index<N>& idx) restrict(amp,cpu);
  extent operator-(const index<N>& idx) restrict(amp,cpu);

#if 0
  template <int N>
  friend bool operator==(const extent<N>& lhs, const extent<N>& rhs) restrict(amp,cpu);
  template <int N>
  friend bool operator!=(const extent<N>& lhs, const extent<N>& rhs) restrict(amp,cpu);

  template <int N> friend extent<N> operator+(const extent<N>& lhs, int rhs) restrict(amp,cpu);
  template <int N> friend extent<N> operator+(int lhs, const extent<N>& rhs) restrict(amp,cpu);
  template <int N> friend extent<N> operator-(const extent<N>& lhs, int rhs) restrict(amp,cpu);
  template <int N> friend extent<N> operator-(int lhs, const extent<N>& rhs) restrict(amp,cpu);
  template <int N> friend extent<N> operator*(const extent<N>& lhs, int rhs) restrict(amp,cpu);
  template <int N> friend extent<N> operator*(int lhs, const extent<N>& rhs) restrict(amp,cpu);
  template <int N> friend extent<N> operator/(const extent<N>& lhs, int rhs) restrict(amp,cpu);
  template <int N> friend extent<N> operator/(int lhs, const extent<N>& rhs) restrict(amp,cpu);
  template <int N> friend extent<N> operator%(const extent<N>& lhs, int rhs) restrict(amp,cpu);
  template <int N> friend extent<N> operator%(int lhs, const extent<N>& rhs) restrict(amp,cpu);
#endif

  extent& operator+=(int rhs) restrict(amp,cpu);
  extent& operator-=(int rhs) restrict(amp,cpu);
  extent& operator*=(int rhs) restrict(amp,cpu);
  extent& operator/=(int rhs) restrict(amp,cpu);
  extent& operator%=(int rhs) restrict(amp,cpu);

  extent& operator++() restrict(amp,cpu);
  extent operator++(int) restrict(amp,cpu);
  extent& operator--() restrict(amp,cpu);
  extent operator--(int) restrict(amp,cpu);
};

template <int D0, int D1/*=0*/, int D2/*=0*/>
class tiled_extent : public extent<3>
{
public:
  static const int rank = 3;
  tiled_extent() restrict(amp,cpu);
  tiled_extent(const tiled_extent& other) restrict(amp,cpu);
  tiled_extent(const extent<3>& extent) restrict(amp,cpu);
  tiled_extent& operator=(const tiled_extent& other) restrict(amp,cpu);
  tiled_extent pad() const restrict(amp,cpu);
  tiled_extent truncate() const restrict(amp,cpu);
  // __declspec(property(get)) extent<3> tile_extent;
  extent<3> get_tile_extent() const;
  static const int tile_dim0 = D0;
  static const int tile_dim1 = D1;
  static const int tile_dim2 = D2;
  friend bool operator==(const tiled_extent& lhs, const tiled_extent& rhs) restrict(amp,cpu);
  friend bool operator!=(const tiled_extent& lhs, const tiled_extent& rhs) restrict(amp,cpu);
};

template <int D0, int D1>
class tiled_extent<D0,D1,0> : public extent<2>
{
public:
  static const int rank = 2;
  tiled_extent() restrict(amp,cpu);
  tiled_extent(const tiled_extent& other) restrict(amp,cpu);
  tiled_extent(const extent<2>& extent) restrict(amp,cpu);
  tiled_extent& operator=(const tiled_extent& other) restrict(amp,cpu);
  tiled_extent pad() const restrict(amp,cpu);
  tiled_extent truncate() const restrict(amp,cpu);
  // __declspec(property(get)) extent<2> tile_extent;
  extent<2> get_tile_extent() const;
  static const int tile_dim0 = D0;
  static const int tile_dim1 = D1;
  friend bool operator==(const tiled_extent& lhs, const tiled_extent& rhs) restrict(amp,cpu);
  friend bool operator!=(const tiled_extent& lhs, const tiled_extent& rhs) restrict(amp,cpu);
};

template <int D0>
class tiled_extent<D0,0,0> : public extent<1>
{
public:
  static const int rank = 1;
  tiled_extent() restrict(amp,cpu);
  tiled_extent(const tiled_extent& other) restrict(amp,cpu);
  tiled_extent(const extent<1>& extent) restrict(amp,cpu);
  tiled_extent& operator=(const tiled_extent& other) restrict(amp,cpu);
  tiled_extent pad() const restrict(amp,cpu);
  tiled_extent truncate() const restrict(amp,cpu);
  // __declspec(property(get)) extent<1> tile_extent;
  extent<1> get_tile_extent() const;
  static const int tile_dim0 = D0;
  friend bool operator==(const tiled_extent& lhs, const tiled_extent& rhs) restrict(amp,cpu);
  friend bool operator!=(const tiled_extent& lhs, const tiled_extent& rhs) restrict(amp,cpu);
};

template <typename T, int N=1>
class array {
public:
  static const int rank = N;
  typedef T value_type;
  array() = delete;

  explicit array(const extent<N>& extent);

  array(const extent<N>& extent, accelerator_view av, accelerator_view associated_av);

  template <typename InputIterator>
  array(const extent<N>& extent, InputIterator srcBegin);

  template <typename InputIterator>
  array(const extent<N>& extent, InputIterator srcBegin, InputIterator srcEnd);

  template <typename InputIterator>
  array(const extent<N>& extent, InputIterator srcBegin,
    accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(const extent<N>& extent, InputIterator srcBegin, InputIterator srcEnd,
    accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(const extent<N>& extent, InputIterator srcBegin, accelerator_view av);

  template <typename InputIterator>
  array(const extent<N>& extent, InputIterator srcBegin, InputIterator srcEnd,
  accelerator_view av);

  explicit array(const array_view<const T,N>& src);

  array(const array_view<const T,N>& src,

  accelerator_view av, accelerator_view associated_av); // staging

  array(const array_view<const T,N>& src, accelerator_view av);

  array(const array& other);

  array(array&& other);

  array& operator=(const array& other);

  array& operator=(array&& other);

  array& operator=(const array_view<const T,N>& src);

  void copy_to(array& dest) const;

  void copy_to(const array_view<T,N>& dest) const;

  // __declspec(property(get)) extent<N> extent;
  extent<N> get_extent() const;

  // __declspec(property(get)) accelerator_view accelerator_view;
  accelerator_view get_accelerator_view() const;

  // __declspec(property(get)) accelerator_view associated_accelerator_view;
  accelerator_view get_associated_accelerator_view() const;

  T& operator[](const index<N>& idx) restrict(amp,cpu);

  const T& operator[](const index<N>& idx) const restrict(amp,cpu);

  array_view<T,N-1> operator[](int i) restrict(amp,cpu);

  array_view<const T,N-1> operator[](int i) const restrict(amp,cpu);

  const T& operator()(const index<N>& idx) const restrict(amp,cpu);

  T& operator()(const index<N>& idx) restrict(amp,cpu);

  array_view<T,N-1> operator()(int i) restrict(amp,cpu);

  array_view<const T,N-1> operator()(int i) const restrict(amp,cpu);

  array_view<T,N> section(const index<N>& idx, const extent<N>& ext) restrict(amp,cpu);

  array_view<const T,N> section(const index<N>& idx, const extent<N>& ext) const restrict(amp,cpu);

  array_view<T,N> section(const index<N>& idx) restrict(amp,cpu);

  array_view<const T,N> section(const index<N>& idx) const restrict(amp,cpu);

  template <typename ElementType>
  array_view<ElementType,1> reinterpret_as() restrict(amp,cpu);

  template <typename ElementType>
  array_view<const ElementType,1> reinterpret_as() const restrict(amp,cpu);

  template <int K>
  array_view<T,K> view_as(const extent<K>& viewExtent) restrict(amp,cpu);

  template <int K>
  array_view<const T,K> view_as(const extent<K>& viewExtent) const restrict(amp,cpu);

  operator std::vector<T>() const;

  T* data() restrict(amp,cpu);

  const T* data() const restrict(amp,cpu);
};

template<typename T>
class array<T,1>
{
public:
  static const int rank = 1;

  typedef T value_type;

  array() = delete;

  explicit array(const extent<1>& extent);

  explicit array(int e0);

  array(const extent<1>& extent,
  accelerator_view av, accelerator_view associated_av); // staging

  array(int e0, accelerator_view av, accelerator_view associated_av); // staging

  array(const extent<1>& extent, accelerator_view av);

  array(int e0, accelerator_view av);

  template <typename InputIterator>
  array(const extent<1>& extent, InputIterator srcBegin);

  template <typename InputIterator>
  array(const extent<1>& extent, InputIterator srcBegin, InputIterator srcEnd);

  template <typename InputIterator>
  array(int e0, InputIterator srcBegin);

  template <typename InputIterator>
  array(int e0, InputIterator srcBegin, InputIterator srcEnd);

  template <typename InputIterator>
  array(const extent<1>& extent, InputIterator srcBegin,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(const extent<1>& extent, InputIterator srcBegin, InputIterator srcEnd,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(int e0, InputIterator srcBegin,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(int e0, InputIterator srcBegin, InputIterator srcEnd,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(const extent<1>& extent, InputIterator srcBegin, accelerator_view av);

  template <typename InputIterator>
  array(const extent<1>& extent, InputIterator srcBegin, InputIterator srcEnd,
  accelerator_view av);

  template <typename InputIterator>
  array(int e0, InputIterator srcBegin, InputIterator srcEnd, accelerator_view av);

  array(const array_view<const T,1>& src);

  array(const array_view<const T,1>& src,
  accelerator_view av, accelerator_view associated_av); // staging

  array(const array_view<const T,1>& src, accelerator_view av);

  array(const array& other);

  array(array&& other);

  array& operator=(const array& other);

  array& operator=(array&& other);

  array& operator=(const array_view<const T,1>& src);

  void copy_to(array& dest) const;

  void copy_to(const array_view<T,1>& dest) const;

  // __declspec(property(get)) extent<1> extent;
  extent<1> get_extent() const;

  // __declspec(property(get)) accelerator_view accelerator_view;
  accelerator_view get_accelerator_view() const;

  T& operator[](const index<1>& idx) restrict(amp,cpu);

  const T& operator[](const index<1>& idx) const restrict(amp,cpu);

  T& operator[](int i0) restrict(amp,cpu);

  const T& operator[](int i0) const restrict(amp,cpu);

  T& operator()(const index<1>& idx) restrict(amp,cpu);

  const T& operator()(const index<1>& idx) const restrict(amp,cpu);

  T& operator()(int i0) restrict(amp,cpu);

  const T& operator()(int i0) const restrict(amp,cpu);

  array_view<T,1> section(const index<1>& idx, const extent<1>& ext) restrict(amp,cpu);

  array_view<const T,1> section(const index<1>& idx, const extent<1>& ext) const
  restrict(amp,cpu);

  array_view<T,1> section(const index<1>& idx) restrict(amp,cpu);

  array_view<const T,1> section(const index<1>& idx) const restrict(amp,cpu);

  array_view<T,1> section(int i0, int e0) restrict(amp,cpu);

  array_view<const T,1> section(int i0, int e0) const restrict(amp,cpu);

  template <typename ElementType>
  array_view<ElementType,1> reinterpret_as() restrict(amp,cpu);

  template <typename ElementType>
  array_view<const ElementType,1> reinterpret_as() const restrict(amp,cpu);

  template <int K>
  array_view<T,K> view_as(const extent<K>& viewExtent) restrict(amp,cpu);

  template <int K>
  array_view<const T,K> view_as(const extent<K>& viewExtent) const restrict(amp,cpu);

  operator std::vector<T>() const;

  T* data() restrict(amp,cpu);

  const T* data() const restrict(amp,cpu);
};

template<typename T>
class array<T,2>
{
public:
  static const int rank = 2;

  typedef T value_type;

  array() = delete;

  explicit array(const extent<2>& extent);

  array(int e0, int e1);

  array(const extent<2>& extent,

  accelerator_view av, accelerator_view associated_av); // staging

  array(int e0, int e1, accelerator_view av, accelerator_view associated_av); // staging

  array(const extent<2>& extent, accelerator_view av);

  array(int e0, int e1, accelerator_view av);

  template <typename InputIterator>
  array(const extent<2>& extent, InputIterator srcBegin);

  template <typename InputIterator>
  array(const extent<2>& extent, InputIterator srcBegin, InputIterator srcEnd);

  template <typename InputIterator>
  array(int e0, int e1, InputIterator srcBegin);

  template <typename InputIterator>
  array(int e0, int e1, InputIterator srcBegin, InputIterator srcEnd);

  template <typename InputIterator>
  array(const extent<2>& extent, InputIterator srcBegin,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(const extent<2>& extent, InputIterator srcBegin, InputIterator srcEnd,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(int e0, int e2, InputIterator srcBegin,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(int e0, int e2, InputIterator srcBegin, InputIterator srcEnd,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(const extent<2>& extent, InputIterator srcBegin, accelerator_view av);

  template <typename InputIterator>
  array(const extent<2>& extent, InputIterator srcBegin, InputIterator srcEnd,
  accelerator_view av);

  template <typename InputIterator>
  array(int e0, int e1, InputIterator srcBegin, accelerator_view av);

  template <typename InputIterator>
  array(int e0, int e1, InputIterator srcBegin, InputIterator srcEnd, accelerator_view av);

  array(const array_view<const T,2>& src);

  array(const array_view<const T,2>& src,
  accelerator_view av, accelerator_view associated_av); // staging

  array(const array_view<const T,2>& src, accelerator_view av);

  array(const array& other);

  array(array&& other);

  array& operator=(const array& other);

  array& operator=(array&& other);

  array& operator=(const array_view<const T,2>& src);

  void copy_to(array& dest) const;

  void copy_to(const array_view<T,2>& dest) const;

  // __declspec(property(get)) extent<2> extent;
  extent<2> get_extent() const;

  // __declspec(property(get)) accelerator_view accelerator_view;
  accelerator_view get_accelerator_view() const;

  T& operator[](const index<2>& idx) restrict(amp,cpu);

  const T& operator[](const index<2>& idx) const restrict(amp,cpu);

  array_view<T,1> operator[](int i0) restrict(amp,cpu);

  array_view<const T,1> operator[](int i0) const restrict(amp,cpu);

  T& operator()(const index<2>& idx) restrict(amp,cpu);

  const T& operator()(const index<2>& idx) const restrict(amp,cpu);

  T& operator()(int i0, int i1) restrict(amp,cpu);

  const T& operator()(int i0, int i1) const restrict(amp,cpu);

  array_view<T,2> section(const index<2>& idx, const extent<2>& ext) restrict(amp,cpu);

  array_view<const T,2> section(const index<2>& idx, const extent<2>& ext) const
  restrict(amp,cpu);

  array_view<T,2> section(const index<2>& idx) restrict(amp,cpu);

  array_view<const T,2> section(const index<2>& idx) const restrict(amp,cpu);

  array_view<T,2> section(int i0, int i1, int e0, int e1) restrict(amp,cpu);

  array_view<const T,2> section(int i0, int i1, int e0, int e1) const restrict(amp,cpu);

  template <typename ElementType>
  array_view<ElementType,1> reinterpret_as() restrict(amp,cpu);

  template <typename ElementType>
  array_view<const ElementType,1> reinterpret_as() const restrict(amp,cpu);

  template <int K>
  array_view<T,K> view_as(const extent<K>& viewExtent) restrict(amp,cpu);

  template <int K>
  array_view<const T,K> view_as(const extent<K>& viewExtent) const restrict(amp,cpu);

  operator std::vector<T>() const;

  T* data() restrict(amp,cpu);

  const T* data() const restrict(amp,cpu);
};

template<typename T>
class array<T,3>
{
public:
  static const int rank = 3;

  typedef T value_type;

  array() = delete;

  explicit array(const extent<3>& extent);

  array(int e0, int e1, int e2);

  array(const extent<3>& extent,

  accelerator_view av, accelerator_view associated_av); // staging
  array(int e0, int e1, int e2,
  accelerator_view av, accelerator_view associated_av); // staging

  array(const extent<3>& extent, accelerator_view av);

  array(int e0, int e1, int e2, accelerator_view av);

  template <typename InputIterator>
  array(const extent<3>& extent, InputIterator srcBegin);

  template <typename InputIterator>
  array(const extent<3>& extent, InputIterator srcBegin, InputIterator srcEnd);

  template <typename InputIterator>
  array(int e0, int e1, int e2, InputIterator srcBegin);

  template <typename InputIterator>
  array(int e0, int e1, int e2, InputIterator srcBegin, InputIterator srcEnd);

  template <typename InputIterator>
  array(const extent<3>& extent, InputIterator srcBegin,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(const extent<3>& extent, InputIterator srcBegin, InputIterator srcEnd,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(int e0, int e1, int e2, InputIterator srcBegin,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(int e0, int e1, int e2, InputIterator srcBegin, InputIterator srcEnd,
  accelerator_view av, accelerator_view associated_av); // staging

  template <typename InputIterator>
  array(const extent<3>& extent, InputIterator srcBegin, accelerator_view av);

  template <typename InputIterator>
  array(const extent<3>& extent, InputIterator srcBegin, InputIterator srcEnd,
  accelerator_view av);

  template <typename InputIterator>
  array(int e0, int e1, int e2, InputIterator srcBegin, accelerator_view av);

  template <typename InputIterator>
  array(int e0, int e1, int e2, InputIterator srcBegin, InputIterator srcEnd,

  accelerator_view av);

  array(const array_view<const T,3>& src);

  array(const array_view<const T,3>& src,

  accelerator_view av, accelerator_view associated_av); // staging

  array(const array_view<const T,3>& src, accelerator_view av);

  array(const array& other);

  array(array&& other);

  array& operator=(const array& other);

  array& operator=(array&& other);

  array& operator=(const array_view<const T,3>& src);

  void copy_to(array& dest) const;

  void copy_to(const array_view<T,3>& dest) const;

  // __declspec(property(get)) extent<3> extent;
  extent<3> get_extent() const;
  // __declspec(property(get)) accelerator_view accelerator_view;
  accelerator_view get_accelerator_view() const;

  T& operator[](const index<3>& idx) restrict(amp,cpu);

  const T& operator[](const index<3>& idx) const restrict(amp,cpu);

  array_view<T,2> operator[](int i0) restrict(amp,cpu);

  array_view<const T,2> operator[](int i0) const restrict(amp,cpu);

  T& operator()(const index<3>& idx) restrict(amp,cpu);

  const T& operator()(const index<3>& idx) const restrict(amp,cpu);

  T& operator()(int i0, int i1, int i2) restrict(amp,cpu);

  const T& operator()(int i0, int i1, int i2) const restrict(amp,cpu);

  array_view<T,3> section(const index<3>& idx, const extent<3>& ext) restrict(amp,cpu);

  array_view<const T,3> section(const index<3>& idx, const extent<3>& ext) const
  restrict(amp,cpu);

  array_view<T,3> section(const index<3>& idx) restrict(amp,cpu);

  array_view<const T,3> section(const index<3>& idx) const restrict(amp,cpu);

  array_view<T,3> section(int i0, int i1, int i2,
  int e0, int e1, int e2) restrict(amp,cpu);

  array_view<const T,3> section(int i0, int i1, int i2,
  int e0, int e1, int e2) const restrict(amp,cpu);

  template <typename ElementType>
  array_view<ElementType,1> reinterpret_as() restrict(amp,cpu);

  template <typename ElementType>
  array_view<const ElementType,1> reinterpret_as() const restrict(amp,cpu);

  template <int K>
  array_view<T,K> view_as(const extent<K>& viewExtent) restrict(amp,cpu);

  template <int K>
  array_view<const T,K> view_as(const extent<K>& viewExtent) const restrict(amp,cpu);

  operator std::vector<T>() const;

  T* data() restrict(amp,cpu);

  const T* data() const restrict(amp,cpu);
};

template <typename T, int N/* = 1*/>
class array_view
{
public:
  static const int rank = N;
  typedef T value_type;

  array_view() = delete;
  array_view(array<T,N>& src) restrict(amp,cpu);
  template <typename Container>
    array_view(const extent<N>& extent, Container& src);
  array_view(const extent<N>& extent, value_type* src) restrict(amp,cpu);

  array_view(const array_view& other) restrict(amp,cpu);

  array_view& operator=(const array_view& other) restrict(amp,cpu);

  void copy_to(array<T,N>& dest) const;
  void copy_to(const array_view& dest) const;

  // __declspec(property(get)) extent<N> extent;
  extent<N> get_extent() const;

  // These are restrict(amp,cpu)
  T& operator[](const index<N>& idx) const restrict(amp,cpu);
  array_view<T,N-1> operator[](int i) const restrict(amp,cpu);

  T& operator()(const index<N>& idx) const restrict(amp,cpu);
  array_view<T,N-1> operator()(int i) const restrict(amp,cpu);

  array_view<T,N> section(const index<N>& idx, const extent<N>& ext) restrict(amp,cpu);
  array_view<T,N> section(const index<N>& idx) const restrict(amp,cpu);

  void synchronize() const;
  completion_future synchronize_async() const;

  void refresh() const;
  void discard_data() const;
};

template <typename T>
class array_view<T,1>
{
public:
  static const int rank = 1;
  typedef T value_type;
  
  array_view() = delete;
  array_view(array<T,1>& src) restrict(amp,cpu);
  template <typename Container>
    array_view(const extent<1>& extent, Container& src);
  template <typename Container>
    array_view(int e0, Container& src);
  array_view(const extent<1>& extent, value_type* src) restrict(amp,cpu);
  array_view(int e0, value_type* src) restrict(amp,cpu);
  
  array_view(const array_view& other) restrict(amp,cpu);
  
  array_view& operator=(const array_view& other) restrict(amp,cpu);

  void copy_to(array<T,1>& dest) const;
  void copy_to(const array_view& dest) const;

  // __declspec(property(get)) extent<1> extent;
  extent<1> get_extent() const;

  T& operator[](const index<1>& idx) const restrict(amp,cpu);
  T& operator[](int i) const restrict(amp,cpu);

  T& operator()(const index<1>& idx) const restrict(amp,cpu);
  T& operator()(int i) const restrict(amp,cpu);
  
  array_view<T,1> section(const index<1>& idx, const extent<1>& ext) const restrict(amp,cpu);
  array_view<T,1> section(const index<1>& idx) const restrict(amp,cpu);
  array_view<T,1> section(const extent<1>& ext) const restrict(amp,cpu);
  array_view<T,1> section(int i0, int e0) restrict(amp,cpu);

  template <typename ElementType>
    array_view<ElementType,1> reinterpret_as() const restrict(amp,cpu);

  template <int K>
    array_view<T,K> view_as(extent<K> viewExtent) const restrict(amp,cpu);

  T* data() const restrict(amp,cpu);

  void synchronize() const;
  completion_future synchronize_async() const;

  void refresh() const;
  void discard_data() const;
};

template <typename T>
class array_view<T,2>
{
public:
  static const int rank = 2;
  typedef T value_type;

  array_view() = delete;
  array_view(array<T,2>& src) restrict(amp,cpu);
  template <typename Container>
    array_view(const extent<2>& extent, Container& src);
  template <typename Container>
    array_view(int e0, int e1, Container& src);
  array_view(const extent<2>& extent, value_type* src) restrict(amp,cpu);
  array_view(int e0, int e1, value_type* src) restrict(amp,cpu);

  array_view(const array_view& other) restrict(amp,cpu);

  array_view& operator=(const array_view& other) restrict(amp,cpu);

  void copy_to(array<T,2>& dest) const;
  void copy_to(const array_view& dest) const;

  // __declspec(property(get)) extent<2> extent;
  extent<2> get_extent() const;

  T& operator[](const index<2>& idx) const restrict(amp,cpu);
  array_view<T,1> operator[](int i) const restrict(amp,cpu);

  T& operator()(const index<2>& idx) const restrict(amp,cpu);
  T& operator()(int i0, int i1) const restrict(amp,cpu);

  
  
  
  
  array_view<T,2> section(const index<2>& idx, const extent<2>& ext) const restrict(amp,cpu);
  array_view<T,2> section(const index<2>& idx) const restrict(amp,cpu);
  array_view<T,2> section(const extent<2>& ext) const restrict(amp,cpu);
  array_view<T,2> section(int i0, int i1, int e0, int e1) const restrict(amp,cpu);

  void synchronize() const;
  completion_future synchronize_async() const;

  void refresh() const;
  void discard_data() const;
};

template <typename T>
class array_view<T,3>
{
public:
  static const int rank = 3;
  typedef T value_type;

  array_view() = delete;
  array_view(array<T,3>& src) restrict(amp,cpu);
  template <typename Container>
    array_view(const extent<3>& extent, Container& src);
  template <typename Container>
    array_view(int e0, int e1, int e2, Container& src);
  array_view(const extent<3>& extent, value_type* src) restrict(amp,cpu);
  array_view(int e0, int e1, int e2, value_type* src) restrict(amp,cpu);

  array_view(const array_view& other) restrict(amp,cpu);

  array_view& operator=(const array_view& other) restrict(amp,cpu);

  void copy_to(array<T,3>& dest) const;
  void copy_to(const array_view& dest) const;

  // __declspec(property(get)) extent<3> extent;
  extent<3> get_extent() const;

  T& operator[](const index<3>& idx) const restrict(amp,cpu);
  array_view<T,2> operator[](int i) const restrict(amp,cpu);

  T& operator()(const index<3>& idx) const restrict(amp,cpu);
  T& operator()(int i0, int i1, int i2) const restrict(amp,cpu);

  array_view<T,3> section(const index<3>& idx, const extent<3>& ext) const restrict(amp,cpu);
  array_view<T,3> section(const index<3>& idx) const restrict(amp,cpu);
  array_view<T,3> section(const extent<3>& ext) const restrict(amp,cpu);
  array_view<T,3> section(int i0, int i1, int i2, int e0, int e1, int e2) const restrict(amp,cpu);

  void synchronize() const;
  completion_future synchronize_async() const;

  void refresh() const;
  void discard_data() const;
};

template <typename T, int N/*=1*/>
class array_view<const T,N>
{
public:
  static const int rank = N;
  typedef const T value_type;

  array_view() = delete;
  array_view(const array<T,N>& src) restrict(amp,cpu);
  template <typename Container>
    array_view(const extent<N>& extent, const Container& src);
  array_view(const extent<N>& extent, const value_type* src) restrict(amp,cpu);
  array_view(const array_view<T,N>& other) restrict(amp,cpu);

  array_view(const array_view<const T,N>& other) restrict(amp,cpu);

  array_view& operator=(const array_view& other) restrict(amp,cpu);

  void copy_to(array<T,N>& dest) const;
  void copy_to(const array_view<T,N>& dest) const;

  // __declspec(property(get)) extent<N> extent;
  extent<N> get_extent() const;

  const T& operator[](const index<N>& idx) const restrict(amp,cpu);
  array_view<const T,N-1> operator[](int i) const restrict(amp,cpu);

  const T& operator()(const index<N>& idx) const restrict(amp,cpu);
  array_view<const T,N-1> operator()(int i) const restrict(amp,cpu);

  array_view<const T,N> section(const index<N>& idx, const extent<N>& ext) const restrict(amp,cpu);
  array_view<const T,N> section(const index<N>& idx) const restrict(amp,cpu);

  void refresh() const;
};

template <typename T>
class array_view<const T,1>
{
public:
  static const int rank = 1;
  typedef const T value_type;

  array_view() = delete;
  array_view(const array<T,1>& src) restrict(amp,cpu);
  template <typename Container>
    array_view(const extent<1>& extent, const Container& src);
  template <typename Container>
    array_view(int e0, const Container& src);
  array_view(const extent<1>& extent, const value_type* src) restrict(amp,cpu);
  array_view(int e0, const value_type* src) restrict(amp,cpu);

  array_view(const array_view<T,1>& other) restrict(amp,cpu);
  array_view(const array_view<const T,1>& other) restrict(amp,cpu);

  array_view& operator=(const array_view& other) restrict(amp,cpu);

  void copy_to(array<T,1>& dest) const;
  void copy_to(const array_view<T,1>& dest) const;

  // __declspec(property(get)) extent<1> extent;
  extent<1> get_extent() const;

  // These are restrict(amp,cpu)
  const T& operator[](const index<1>& idx) const restrict(amp,cpu);
  const T& operator[](int i) const restrict(amp,cpu);

  const T& operator()(const index<1>& idx) const restrict(amp,cpu);
  const T& operator()(int i) const restrict(amp,cpu);
  
  array_view<const T,1> section(const index<1>& idx, const extent<1>& ext) const restrict(amp,cpu);
  array_view<const T,1> section(const index<1>& idx) const restrict(amp,cpu);
  array_view<const T,1> section(const extent<1>& ext) const restrict(amp,cpu);
  array_view<const T,1> section(int i0, int e0) const restrict(amp,cpu);

  template <typename ElementType>
    array_view<const ElementType,1> reinterpret_as() const restrict(amp,cpu);
  template <int K>
    array_view<const T,K> view_as(extent<K> viewExtent) const restrict(amp,cpu);

  const T* data() const restrict(amp,cpu);
  void refresh() const;
};

template <typename T>
class array_view<const T,2>
{
public:
  static const int rank = 2;
  typedef const T value_type;

  array_view() = delete;
  array_view(const array<T,2>& src) restrict(amp,cpu);
  template <typename Container>
  array_view(const extent<2>& extent, const Container& src);
  template <typename Container>
  array_view(int e0, int e1, const Container& src);
  array_view(const extent<2>& extent, const value_type* src) restrict(amp,cpu);
  array_view(int e0, int e1, const value_type* src) restrict(amp,cpu);

  array_view(const array_view<T,2>& other) restrict(amp,cpu);
  array_view(const array_view<const T,2>& other) restrict(amp,cpu);

  array_view& operator=(const array_view& other) restrict(amp,cpu);

  void copy_to(array<T,2>& dest) const;
  void copy_to(const array_view<T,2>& dest) const;

  // __declspec(property(get)) extent<2> extent;
  extent<2> get_extent() const;

  const T& operator[](const index<2>& idx) const restrict(amp,cpu);
  array_view<const T,1> operator[](int i) const restrict(amp,cpu);

  const T& operator()(const index<2>& idx) const restrict(amp,cpu);
  const T& operator()(int i0, int i1) const restrict(amp,cpu);

  array_view<const T,2> section(const index<2>& idx, const extent<2>& ext) const restrict(amp,cpu);
  array_view<const T,2> section(const index<2>& idx) const restrict(amp,cpu);
  array_view<const T,2> section(const extent<2>& ext) const restrict(amp,cpu);
  array_view<const T,2> section(int i0, int i1, int e0, int e1) const restrict(amp,cpu);

  void refresh() const;
};

template <typename T>
class array_view<const T,3>
{
public:
  static const int rank = 3;
  typedef const T value_type;

  array_view() = delete;
  array_view(const array<T,3>& src) restrict(amp,cpu);
  template <typename Container>
    array_view(const extent<3>& extent, const Container& src);
  template <typename Container>
    array_view(int e0, int e1, int e2, const Container& src);
  array_view(const extent<3>& extent, const value_type* src) restrict(amp,cpu);
  array_view(int e0, int e1, int e2, const value_type* src) restrict(amp,cpu);

  array_view(const array_view<T,3>& other) restrict(amp,cpu);
  array_view(const array_view<const T,3>& other) restrict(amp,cpu);

  array_view& operator=(const array_view& other) restrict(amp,cpu);

  void copy_to(array<T,3>& dest) const;
  void copy_to(const array_view<T,3>& dest) const;

  // __declspec(property(get)) extent<3> extent;
  extent<3> get_extent() const;

  // These are restrict(amp,cpu)
  const T& operator[](const index<3>& idx) const restrict(amp,cpu);
  array_view<const T,2> operator[](int i) const restrict(amp,cpu);

  const T& operator()(const index<3>& idx) const restrict(amp,cpu);
  const T& operator()(int i0, int i1, int i2) const restrict(amp,cpu);

  array_view<const T,3> section(const index<3>& idx, const extent<3>& ext) const restrict(amp,cpu);
  array_view<const T,3> section(const index<3>& idx) const restrict(amp,cpu);
  array_view<const T,3> section(const extent<3>& ext) const restrict(amp,cpu);
  array_view<const T,3> section(int i0, int i1, int i2, int e0, int e1, int e2) const restrict(amp,cpu);

  void refresh() const;
};

template <int N, typename Kernel>
void parallel_for_each(extent<N> compute_domain, const Kernel& f);

template <int D0, int D1, int D2, typename Kernel>
void parallel_for_each(tiled_extent<D0,D1,D2> compute_domain, const Kernel& f);

template <int D0, int D1, typename Kernel>
void parallel_for_each(tiled_extent<D0,D1> compute_domain, const Kernel& f);

template <int D0, typename Kernel>
void parallel_for_each(tiled_extent<D0> compute_domain, const Kernel& f);

template <int N, typename Kernel>
void parallel_for_each(const accelerator_view& accl_view, extent<N> compute_domain, const Kernel& f);

template <int D0, int D1, int D2, typename Kernel>
void parallel_for_each(const accelerator_view& accl_view, tiled_extent<D0,D1,D2> compute_domain, const Kernel& f);

template <int D0, int D1, typename Kernel>
void parallel_for_each(const accelerator_view& accl_view, tiled_extent<D0,D1> compute_domain, const Kernel& f);

template <int D0, typename Kernel>
void parallel_for_each(const accelerator_view& accl_view, tiled_extent<D0> compute_domain, const Kernel& f);


} // namespace Concurrency

