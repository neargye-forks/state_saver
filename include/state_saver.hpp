//   _____ _        _          _____                         _____
//  / ____| |      | |        / ____|                       / ____|_     _
// | (___ | |_ __ _| |_ ___  | (___   __ ___   _____ _ __  | |   _| |_ _| |_
//  \___ \| __/ _` | __/ _ \  \___ \ / _` \ \ / / _ \ '__| | |  |_   _|_   _|
//  ____) | || (_| | ||  __/  ____) | (_| |\ V /  __/ |    | |____|_|   |_|
// |_____/ \__\__,_|\__\___| |_____/ \__,_| \_/ \___|_|     \_____|
// https://github.com/Neargye/state_saver
// vesion 0.4.4
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2018 - 2019 Daniil Goncharov <neargye@gmail.com>.
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <type_traits>
#if (defined(_MSC_VER) && _MSC_VER >= 1900) || ((defined(__clang__) || defined(__GNUC__)) && __cplusplus >= 201700L)
#include <exception>
#endif

// state_saver throwable settings:
// STATE_SAVER_MAY_EXCEPTIONS
// STATE_SAVER_NO_EXCEPTIONS
// STATE_SAVER_SUPPRESS_EXCEPTIONS

// state_saver assignable settings:
// STATE_SAVER_FORCE_MOVE_ASSIGNABLE
// STATE_SAVER_FORCE_COPY_ASSIGNABLE

#if !defined(STATE_SAVER_MAY_EXCEPTIONS) && !defined(STATE_SAVER_NO_EXCEPTIONS) && !defined(STATE_SAVER_SUPPRESS_EXCEPTIONS)
#  define STATE_SAVER_MAY_EXCEPTIONS
#elif (defined(STATE_SAVER_MAY_EXCEPTIONS) + defined(STATE_SAVER_NO_EXCEPTIONS) + defined(STATE_SAVER_SUPPRESS_EXCEPTIONS)) > 1
#  error Only one of STATE_SAVER_MAY_EXCEPTIONS and STATE_SAVER_NO_EXCEPTIONS and STATE_SAVER_SUPPRESS_EXCEPTIONS may be defined.
#endif

#if (defined(STATE_SAVER_FORCE_MOVE_ASSIGNABLE) + defined(STATE_SAVER_FORCE_COPY_ASSIGNABLE)) > 1
#  error Only one of STATE_SAVER_FORCE_MOVE_ASSIGNABLE and STATE_SAVER_FORCE_COPY_ASSIGNABLE may be defined.
#endif

#if defined(STATE_SAVER_SUPPRESS_EXCEPTIONS)
#  define STATE_SAVER_NOEXCEPT(...) noexcept
#  define STATE_SAVER_TRY try {
#  define STATE_SAVER_CATCH } catch (...) {}
#else
#  define STATE_SAVER_NOEXCEPT(...) noexcept(__VA_ARGS__)
#  define STATE_SAVER_TRY
#  define STATE_SAVER_CATCH
#endif

namespace state_saver {

namespace detail {

#if defined(_MSC_VER) && _MSC_VER < 1900
inline int uncaught_exceptions() noexcept {
  return *(reinterpret_cast<int*>(static_cast<char*>(static_cast<void*>(_getptd())) + (sizeof(void*) == 8 ? 0x100 : 0x90)));
}
#elif (defined(__clang__) || defined(__GNUC__)) && __cplusplus < 201700L
struct __cxa_eh_globals;
extern "C" __cxa_eh_globals* __cxa_get_globals() noexcept;
inline int uncaught_exceptions() noexcept {
  return *(reinterpret_cast<unsigned int*>(static_cast<char*>(static_cast<void*>(__cxa_get_globals())) + sizeof(void*)));
}
#else
inline int uncaught_exceptions() noexcept {
  return std::uncaught_exceptions();
}
#endif

class on_exit_policy final {
  bool restore_;

 public:
  explicit on_exit_policy(bool restore)
      : restore_(restore) {}

  on_exit_policy() = delete;
  on_exit_policy(const on_exit_policy&) = default;
  on_exit_policy(on_exit_policy&&) = default;
  on_exit_policy& operator=(const on_exit_policy&) = default;
  on_exit_policy& operator=(on_exit_policy&&) = default;
  ~on_exit_policy() = default;

  void dismiss() noexcept {
    restore_ = false;
  }

  bool should_restore() const noexcept {
    return restore_;
  }
};

class on_fail_policy final {
  int ec_;

 public:
  explicit on_fail_policy(bool restore)
      : ec_(restore ? uncaught_exceptions() : -1) {}

  on_fail_policy() = delete;
  on_fail_policy(const on_fail_policy&) = default;
  on_fail_policy(on_fail_policy&&) = default;
  on_fail_policy& operator=(const on_fail_policy&) = default;
  on_fail_policy& operator=(on_fail_policy&&) = default;
  ~on_fail_policy() = default;

  void dismiss() noexcept {
    ec_ = -1;
  }

  bool should_restore() const noexcept {
    return ec_ != -1 && ec_ < uncaught_exceptions();
  }
};

class on_success_policy final {
  int ec_;

 public:
  explicit on_success_policy(bool restore)
      : ec_(restore ? uncaught_exceptions() : -1) {}

  on_success_policy() = delete;
  on_success_policy(const on_success_policy&) = default;
  on_success_policy(on_success_policy&&) = default;
  on_success_policy& operator=(const on_success_policy&) = default;
  on_success_policy& operator=(on_success_policy&&) = default;
  ~on_success_policy() = default;

  void dismiss() noexcept {
    ec_ = -1;
  }

  bool should_restore() const noexcept {
    return ec_ != -1 && ec_ >= uncaught_exceptions();
  }
};

template <typename U, typename P>
class state_saver final {
  using T = typename std::remove_reference<U>::type;
#if defined(STATE_SAVER_FORCE_MOVE_ASSIGNABLE)
  using assignable_t = T&&;
#elif defined(STATE_SAVER_FORCE_COPY_ASSIGNABLE)
  using assignable_t = T&;
#else
  using assignable_t = typename std::conditional<
      std::is_nothrow_assignable<T&, T&&>::value ||
          !std::is_assignable<T&, T&>::value ||
          (!std::is_nothrow_assignable<T&, T&>::value && std::is_assignable<T&, T&&>::value),
      T&&, T&>::type;
#endif

  static_assert(!std::is_const<T>::value,
                "state_saver require not const type.");
  static_assert(!std::is_rvalue_reference<U>::value && (std::is_lvalue_reference<U>::value || std::is_same<T, U>::value),
                "state_saver require lvalue type.");
  static_assert(!std::is_array<T>::value,
                "state_saver require not array type.");
  static_assert(!std::is_pointer<T>::value,
                "state_saver require not pointer type.");
  static_assert(!std::is_function<T>::value,
                "state_saver require not function type.");
  static_assert(std::is_constructible<T, T&>::value,
                "state_saver require copy constructible.");
  static_assert(std::is_assignable<T&, assignable_t>::value,
                "state_saver require operator=.");
#if defined(STATE_SAVER_NO_EXCEPTIONS)
  static_assert(std::is_nothrow_assignable<T&, assignable_t>::value,
                "state_saver require noexcept operator=.");
#endif
  static_assert(std::is_same<P, on_exit_policy>::value ||
                    std::is_same<P, on_fail_policy>::value ||
                    std::is_same<P, on_success_policy>::value,
                "state_saver require on_exit_policy, on_fail_policy or on_success_policy.");

 public:
  state_saver() = delete;
  state_saver(const state_saver&) = delete;
  state_saver(state_saver&&) = delete;
  state_saver& operator=(const state_saver&) = delete;
  state_saver& operator=(state_saver&&) = delete;

  state_saver(T&&) = delete;
  state_saver(const T&) = delete;

  explicit state_saver(T& object) noexcept(std::is_nothrow_constructible<T, T&>::value)
      : policy_(true),
        previous_ref_(object),
        previous_value_(object) {}

  void dismiss() noexcept {
    policy_.dismiss();
  }

  void restore() STATE_SAVER_NOEXCEPT(std::is_nothrow_assignable<T&, T&>::value) {
    static_assert(std::is_assignable<T&, T&>::value, "state_saver::restore require copy operator=.");
    STATE_SAVER_TRY
      previous_ref_ = previous_value_;
    STATE_SAVER_CATCH
  }

  ~state_saver() STATE_SAVER_NOEXCEPT(std::is_nothrow_assignable<T&, assignable_t>::value) {
    if (policy_.should_restore()) {
      STATE_SAVER_TRY
        previous_ref_ = static_cast<assignable_t>(previous_value_);
      STATE_SAVER_CATCH
    }
  }

 private:
  P policy_;
  T& previous_ref_;
  T previous_value_;
};

} // namespace detail

template <typename T>
using state_saver_exit = detail::state_saver<T, detail::on_exit_policy>;

template <typename T>
using state_saver_fail = detail::state_saver<T, detail::on_fail_policy>;

template <typename T>
using state_saver_succes = detail::state_saver<T, detail::on_success_policy>;

} // namespace state_saver

// ATTR_MAYBE_UNUSED suppresses compiler warnings on unused entities, if any.
#if !defined(ATTR_MAYBE_UNUSED)
#  if defined(__clang__)
#    if (__clang_major__ * 10 + __clang_minor__) >= 39 && __cplusplus >= 201703L
#      define ATTR_MAYBE_UNUSED [[maybe_unused]]
#    else
#      define ATTR_MAYBE_UNUSED __attribute__((__unused__))
#    endif
#  elif defined(__GNUC__)
#    if __GNUC__ >= 7 && __cplusplus >= 201703L
#      define ATTR_MAYBE_UNUSED [[maybe_unused]]
#    else
#      define ATTR_MAYBE_UNUSED __attribute__((__unused__))
#    endif
#  elif defined(_MSC_VER)
#    if _MSC_VER >= 1911 && defined(_MSVC_LANG) && _MSVC_LANG >= 201703L
#      define ATTR_MAYBE_UNUSED [[maybe_unused]]
#    else
#      define ATTR_MAYBE_UNUSED __pragma(warning(suppress : 4100 4101 4189))
#    endif
#  else
#    define ATTR_MAYBE_UNUSED
#  endif
#endif

#define STATE_SAVER_STR_CONCAT_(s1, s2) s1##s2
#define STATE_SAVER_STR_CONCAT(s1, s2) STATE_SAVER_STR_CONCAT_(s1, s2)

#if defined(__COUNTER__)
#  define STATE_SAVER_COUNTER __COUNTER__
#elif defined(__LINE__)
#  define STATE_SAVER_COUNTER __LINE__
#else
#  error state_saver not supported by compiler.
#endif

// Saves the origin variable value and restores on scope exit, undoes any changes that could occure to the object.
#define MAKE_STATE_SAVER_EXIT(name, x) ::state_saver::state_saver_exit<decltype(x)> name{x};
#define STATE_SAVER_EXIT(x) \
  ATTR_MAYBE_UNUSED const   \
  MAKE_STATE_SAVER_EXIT(STATE_SAVER_STR_CONCAT(__state_saver_exit__object_, STATE_SAVER_COUNTER), x);

// Saves the origin variable value and restores on scope exit when an exception has been thrown before the block's end, undoes any changes that could occure to the object.
#define MAKE_STATE_SAVER_FAIL(name, x) ::state_saver::state_saver_fail<decltype(x)> name{x};
#define STATE_SAVER_FAIL(x) \
  ATTR_MAYBE_UNUSED const   \
  MAKE_STATE_SAVER_FAIL(STATE_SAVER_STR_CONCAT(__state_saver_fail__object_, STATE_SAVER_COUNTER), x);

// Saves the origin variable value and restores on scope exit when no exceptions have been thrown, undoes any changes that could occure to the object.
#define MAKE_STATE_SAVER_SUCCESS(name, x) ::state_saver::state_saver_succes<decltype(x)> name{x};
#define STATE_SAVER_SUCCESS(x) \
  ATTR_MAYBE_UNUSED const      \
  MAKE_STATE_SAVER_SUCCESS(STATE_SAVER_STR_CONCAT(__state_saver_succes__object_, STATE_SAVER_COUNTER), x);
