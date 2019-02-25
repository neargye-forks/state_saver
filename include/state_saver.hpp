//   _____ _        _          _____                         _____
//  / ____| |      | |        / ____|                       / ____|_     _
// | (___ | |_ __ _| |_ ___  | (___   __ ___   _____ _ __  | |   _| |_ _| |_
//  \___ \| __/ _` | __/ _ \  \___ \ / _` \ \ / / _ \ '__| | |  |_   _|_   _|
//  ____) | || (_| | ||  __/  ____) | (_| |\ V /  __/ |    | |____|_|   |_|
// |_____/ \__\__,_|\__\___| |_____/ \__,_| \_/ \___|_|     \_____|
// https://github.com/Neargye/state_saver
// vesion 0.3.0-rc
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

// state_saver throwable settings:
// STATE_SAVER_MAY_EXCEPTIONS
// STATE_SAVER_NO_EXCEPTIONS
// STATE_SAVER_SUPPRESS_EXCEPTIONS

// state_saver assignable settings:
// STATE_SAVER_FORCE_MOVE_ASSIGNABLE
// STATE_SAVER_FORCE_COPY_ASSIGNABLE

#if !defined(STATE_SAVER_MAY_EXCEPTIONS) || !defined(STATE_SAVER_NO_EXCEPTIONS) || !defined(STATE_SAVER_SUPPRESS_EXCEPTIONS)
#  define STATE_SAVER_MAY_EXCEPTIONS
#elif defined(STATE_SAVER_MAY_EXCEPTIONS) && defined(STATE_SAVER_NO_EXCEPTIONS)
#  error only one of STATE_SAVER_MAY_EXCEPTIONS and STATE_SAVER_NO_EXCEPTIONS may be defined.
#elif defined(STATE_SAVER_MAY_EXCEPTIONS) && defined(STATE_SAVER_SUPPRESS_EXCEPTIONS)
#  error only one of STATE_SAVER_MAY_EXCEPTIONS and STATE_SAVER_SUPPRESS_EXCEPTIONS may be defined.
#elif defined(STATE_SAVER_NO_EXCEPTIONS) && defined(STATE_SAVER_SUPPRESS_EXCEPTIONS)
#  error only one of STATE_SAVER_NO_EXCEPTIONS and STATE_SAVER_SUPPRESS_EXCEPTIONS may be defined.
#endif

#if defined(STATE_SAVER_FORCE_MOVE_ASSIGNABLE) && defined(STATE_SAVER_FORCE_COPY_ASSIGNABLE)
#  error only one of STATE_SAVER_FORCE_MOVE_ASSIGNABLE and STATE_SAVER_FORCE_COPY_ASSIGNABLE may be defined.
#endif

namespace yal {

template <typename U>
class state_saver final {
  using T = typename std::remove_reference<U>::type;

  using is_nothrow_assignable = std::integral_constant<bool, std::is_nothrow_assignable<T&, T>::value || std::is_nothrow_assignable<T&, T&>::value>;

#if defined(STATE_SAVER_FORCE_MOVE_ASSIGNABLE)
  using assignable_t = T&&;
#elif defined(STATE_SAVER_FORCE_COPY_ASSIGNABLE)
  using assignable_t = T&;
#else
  using assignable_t = typename std::conditional<
      std::is_nothrow_assignable<T&, T>::value ||
          !std::is_assignable<T&, T&>::value ||
          (!std::is_nothrow_assignable<T&, T&>::value && std::is_assignable<T&, T>::value),
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
  static_assert(std::is_assignable<T&, T>::value || std::is_assignable<T&, T&>::value,
                "state_saver require operator=.");

#if STATE_SAVER_REQUIRE == STATE_SAVER_NO_EXCEPTIONS
  static_assert(is_nothrow_assignable::value,
                "state_saver require noexcept operator=.");
#endif

 public:
  state_saver() = delete;
  state_saver(const state_saver&) = delete;
  state_saver(state_saver&&) = delete;
  state_saver& operator=(const state_saver&) = delete;
  state_saver& operator=(state_saver&&) = delete;

  state_saver(T&&) = delete;
  state_saver(const T&) = delete;

  explicit state_saver(T& object) noexcept(std::is_nothrow_constructible<T, T&>::value)
      : restore_(true),
        previous_ref_(object),
        previous_value_(object) {}

  void dismiss() noexcept {
    restore_ = false;
  }

#if defined(STATE_SAVER_MAY_EXCEPTIONS)
  template <typename = typename std::enable_if<std::is_assignable<T&, T&>::value>::type>
  void restore(bool force = true) noexcept(std::is_nothrow_assignable<T&, T&>::value) {
    if (restore_ || force) {
      previous_ref_ = previous_value_;
    }
  }

  ~state_saver() noexcept(is_nothrow_assignable::value) {
    if (restore_) {
      previous_ref_ = static_cast<assignable_t>(previous_value_);
    }
  }
#elif defined(STATE_SAVER_NO_EXCEPTIONS)
  template <typename = typename std::enable_if<std::is_assignable<T&, T&>::value>::type>
  void restore(bool force = true) noexcept {
    if (restore_ || force) {
      previous_ref_ = previous_value_;
    }
  }

  ~state_saver() noexcept {
    if (restore_) {
      previous_ref_ = static_cast<assignable_t>(previous_value_);
    }
  }
#elif defined(STATE_SAVER_SUPPRESS_EXCEPTIONS)
  template <typename = typename std::enable_if<std::is_assignable<T&, T&>::value>::type>
  void restore(bool force = true) noexcept {
    if (restore_ || force) {
      try {
        previous_ref_ = previous_value_;
      } catch (...) {}
    }
  }

  ~state_saver() noexcept {
    if (restore_) {
      try {
        previous_ref_ = static_cast<assignable_t>(previous_value_);
      } catch (...) {}
    }
  }
#endif

 private:
  bool restore_;
  T& previous_ref_;
  T previous_value_;
};

#if defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 201611L
template <typename T>
state_saver(T&) -> state_saver<T>;
#endif

} // namespace yal

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
#    if _MSC_VER >= 1911 && defined(_MSVC_LANG) &&_MSVC_LANG >= 201703L
#      define ATTR_MAYBE_UNUSED [[maybe_unused]]
#    else
#      define ATTR_MAYBE_UNUSED __pragma(warning(suppress : 4100 4101 4189))
#    endif
#  else
#    define ATTR_MAYBE_UNUSED
#  endif
#endif

#if !defined(STR_CONCAT_)
#  define STR_CONCAT_(s1, s2) s1##s2
#endif

#if !defined(STR_CONCAT)
#  define STR_CONCAT(s1, s2) STR_CONCAT_(s1, s2)
#endif

#define MAKE_STATE_SAVER(name, x) \
  ::yal::state_saver<decltype(x)> (name){x};

#if defined(__COUNTER__)
#  define STATE_SAVER(x)    \
    ATTR_MAYBE_UNUSED const \
    MAKE_STATE_SAVER(STR_CONCAT(__state_saver__object__, __COUNTER__), x);
#elif defined(__LINE__)
#  define STATE_SAVER(x)    \
    ATTR_MAYBE_UNUSED const \
    MAKE_STATE_SAVER(STR_CONCAT(__state_saver__object__, __LINE__), x);
#endif
