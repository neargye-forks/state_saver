//   _____ _        _          _____                         _____
//  / ____| |      | |        / ____|                       / ____|_     _
// | (___ | |_ __ _| |_ ___  | (___   __ ___   _____ _ __  | |   _| |_ _| |_
//  \___ \| __/ _` | __/ _ \  \___ \ / _` \ \ / / _ \ '__| | |  |_   _|_   _|
//  ____) | || (_| | ||  __/  ____) | (_| |\ V /  __/ |    | |____|_|   |_|
// |_____/ \__\__,_|\__\___| |_____/ \__,_| \_/ \___|_|     \_____|
// https://github.com/Neargye/state_saver
// vesion 0.1.2
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
#include <utility>

namespace state_saver {

template <typename T>
class StateSaver final {
  static_assert(::std::is_copy_constructible<T>::value ||
                ::std::is_copy_constructible<T&>::value,
                "StateSaver requirement copy constructible");
  static_assert(::std::is_move_assignable<T>::value ||
                ::std::is_copy_assignable<T>::value ||
                ::std::is_copy_assignable<T&>::value,
                "StateSaver requirement operator =");
  static_assert(!::std::is_pointer<T>::value,
                "StateSaver requirement not pointer type");
  static_assert(!::std::is_function<T>::value,
                "StateSaver requirement not function type");

 public:
  StateSaver() = delete;
  StateSaver(const StateSaver&) = delete;
  StateSaver(StateSaver&&) = delete;
  StateSaver& operator=(const StateSaver&) = delete;
  StateSaver& operator=(StateSaver&&) = delete;

  StateSaver(T&& object) = delete;
  StateSaver(const T& object) = delete;

  inline explicit StateSaver(T& object) noexcept(::std::is_nothrow_copy_constructible<T>::value ||
                                                 ::std::is_nothrow_copy_constructible<T&>::value)
      : restore_{true},
        previous_ref_{object},
        previous_value_{object} {}

  inline void Dismiss() noexcept {
    restore_ = false;
  }

  inline ~StateSaver() noexcept(::std::is_nothrow_move_assignable<T>::value ||
                                ::std::is_nothrow_copy_assignable<T>::value ||
                                ::std::is_nothrow_copy_assignable<T&>::value) {
    using AssignableType = typename ::std::conditional<
        ::std::is_nothrow_move_assignable<T>::value ||
            !(::std::is_copy_assignable<T>::value ||
              ::std::is_copy_assignable<T&>::value) ||
            (!(::std::is_nothrow_copy_assignable<T>::value ||
               ::std::is_nothrow_copy_assignable<T&>::value) &&
             ::std::is_move_assignable<T>::value),
        T&&, T&>::type;

    if (restore_)
      previous_ref_ = static_cast<AssignableType>(previous_value_);
  }

 private:
  bool restore_;
  T& previous_ref_;
  T previous_value_;
};

} // namespace state_saver

#if !defined(CPP_HAS_ATTRIBUTE)
#  if defined(__has_cpp_attribute)
#    define CPP_HAS_ATTRIBUTE(x) __has_cpp_attribute(x)
#  else
#    define CPP_HAS_ATTRIBUTE(x) 0
#  endif
#endif

// CPP_ATTRIBUTE_UNUSED indicates that a function, variable or parameter might or might not be used.
#if !defined(CPP_ATTRIBUTE_UNUSED)
#  if defined(_MSC_VER)
#    if CPP_HAS_ATTRIBUTE(maybe_unused) || (_MSC_VER >= 1911 && _MSVC_LANG >= 201703L)
#      define CPP_ATTRIBUTE_UNUSED [[maybe_unused]]
#    else
#      define CPP_ATTRIBUTE_UNUSED __pragma(warning(suppress : 4100 4101 4189))
#    endif
#  elif defined(__GNUC__) || defined(__clang__)
#    if (__cplusplus >= 201703L) && CPP_HAS_ATTRIBUTE(maybe_unused)
#      define CPP_ATTRIBUTE_UNUSED [[maybe_unused]]
#    else
#      define CPP_ATTRIBUTE_UNUSED __attribute__((unused))
#    endif
#  else
#    define CPP_ATTRIBUTE_UNUSED
#  endif
#endif

#if !defined(STR_CONCAT_)
#  define STR_CONCAT_(s1, s2) s1##s2
#endif

#if !defined(STR_CONCAT)
#  define STR_CONCAT(s1, s2) STR_CONCAT_(s1, s2)
#endif

#define MAKE_STATE_SAVER(state_saver_name, x)                \
  ::state_saver::StateSaver<::std::decay<decltype(x)>::type> \
  state_saver_name{(x)};

#if defined(__COUNTER__)
#  define STATE_SAVER(x)                                       \
    CPP_ATTRIBUTE_UNUSED                                       \
    ::state_saver::StateSaver<::std::decay<decltype(x)>::type> \
    STR_CONCAT(__state_saver__object__, __COUNTER__){(x)};
#elif defined(__LINE__)
#  define STATE_SAVER(x)                                       \
    CPP_ATTRIBUTE_UNUSED                                       \
    ::state_saver::StateSaver<::std::decay<decltype(x)>::type> \
    STR_CONCAT(__state_saver__object__, __LINE__){(x)};
#endif
