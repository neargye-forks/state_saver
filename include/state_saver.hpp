//   _____ _        _          _____                         _____
//  / ____| |      | |        / ____|                       / ____|_     _
// | (___ | |_ __ _| |_ ___  | (___   __ ___   _____ _ __  | |   _| |_ _| |_
//  \___ \| __/ _` | __/ _ \  \___ \ / _` \ \ / / _ \ '__| | |  |_   _|_   _|
//  ____) | || (_| | ||  __/  ____) | (_| |\ V /  __/ |    | |____|_|   |_|
// |_____/ \__\__,_|\__\___| |_____/ \__,_| \_/ \___|_|     \_____|
// https://github.com/Neargye/state_saver
// vesion 0.2.2
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// Copyright (c) 2018 Daniil Goncharov <neargye@gmail.com>.
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

namespace state_saver {

template <typename U>
class StateSaver final {
  using T = typename std::remove_reference<U>::type;

  static_assert(!std::is_const<T>::value,
                "StateSaver requirement not const type.");
  static_assert(!std::is_rvalue_reference<U>::value &&
                    (std::is_lvalue_reference<U>::value || std::is_same<T, U>::value),
                "StateSaver requirement lvalue type.");
  static_assert(!std::is_array<T>::value,
                "StateSaver requirement not array type.");
  static_assert(!std::is_pointer<T>::value,
                "StateSaver requirement not pointer type.");
  static_assert(!std::is_function<T>::value,
                "StateSaver requirement not function type.");
  static_assert(std::is_constructible<T, T&>::value,
                "StateSaver requirement copy constructible.");
  static_assert(std::is_assignable<T&, T>::value || std::is_assignable<T&, T&>::value,
                "StateSaver requirement operator=.");

 public:
  StateSaver() = delete;
  StateSaver(const StateSaver&) = delete;
  StateSaver(StateSaver&&) = delete;
  StateSaver& operator=(const StateSaver&) = delete;
  StateSaver& operator=(StateSaver&&) = delete;

  StateSaver(T&&) = delete;
  StateSaver(const T&) = delete;

  explicit StateSaver(T& object) noexcept(std::is_nothrow_constructible<T, T&>::value)
      : restore_(true),
        previous_ref_(object),
        previous_value_(object) {}

  void Dismiss() noexcept {
    restore_ = false;
  }

  template <typename = typename std::enable_if<std::is_assignable<T&, T&>::value>::type>
  void Restore(bool force = true) noexcept(std::is_nothrow_assignable<T&, T&>::value) {
    if (restore_ || force) {
      previous_ref_ = previous_value_;
    }
  }

  ~StateSaver() noexcept(std::is_nothrow_assignable<T&, T>::value ||
                         std::is_nothrow_assignable<T&, T&>::value) {
    using AssignableType = typename std::conditional<
        std::is_nothrow_assignable<T&, T>::value ||
            !std::is_assignable<T&, T&>::value ||
            (!std::is_nothrow_assignable<T&, T&>::value && std::is_assignable<T&, T>::value),
        T&&, T&>::type;

    if (restore_) {
      previous_ref_ = static_cast<AssignableType>(previous_value_);
    }
  }

 private:
  bool restore_;
  T& previous_ref_;
  T previous_value_;
};

#if defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 201611L
template <typename T>
StateSaver(T&) -> StateSaver<T>;
#endif

} // namespace state_saver

// ATTR_MAYBE_UNUSED suppresses compiler warnings on unused entities, if any.
#if !defined(ATTR_MAYBE_UNUSED)
#  if defined(_MSC_VER)
#    if (_MSC_VER >= 1911 && defined(_MSVC_LANG) &&_MSVC_LANG >= 201703L)
#      define ATTR_MAYBE_UNUSED [[maybe_unused]]
#    else
#      define ATTR_MAYBE_UNUSED __pragma(warning(suppress : 4100 4101 4189))
#    endif
#  elif defined(__clang__)
#    if ((__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 9)) && __cplusplus >= 201703L)
#      define ATTR_MAYBE_UNUSED [[maybe_unused]]
#    else
#      define ATTR_MAYBE_UNUSED __attribute__((__unused__))
#    endif
#  elif defined(__GNUC__)
#    if (__GNUC__ > 7 && __cplusplus >= 201703L)
#      define ATTR_MAYBE_UNUSED [[maybe_unused]]
#    else
#      define ATTR_MAYBE_UNUSED __attribute__((__unused__))
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
  ::state_saver::StateSaver<decltype(x)> (name){x};

#if defined(__COUNTER__)
#  define STATE_SAVER(x)    \
    ATTR_MAYBE_UNUSED const \
    MAKE_STATE_SAVER(STR_CONCAT(__state_saver__object__, __COUNTER__), x);
#elif defined(__LINE__)
#  define STATE_SAVER(x)    \
    ATTR_MAYBE_UNUSED const \
    MAKE_STATE_SAVER(STR_CONCAT(__state_saver__object__, __LINE__), x);
#endif
