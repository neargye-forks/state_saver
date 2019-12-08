//   _____ _        _          _____                         _____
//  / ____| |      | |        / ____|                       / ____|_     _
// | (___ | |_ __ _| |_ ___  | (___   __ ___   _____ _ __  | |   _| |_ _| |_
//  \___ \| __/ _` | __/ _ \  \___ \ / _` \ \ / / _ \ '__| | |  |_   _|_   _|
//  ____) | || (_| | ||  __/  ____) | (_| |\ V /  __/ |    | |____|_|   |_|
// |_____/ \__\__,_|\__\___| |_____/ \__,_| \_/ \___|_|     \_____|
// https://github.com/Neargye/state_saver
// version 0.6.0
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

#ifndef NEARGYE_STATE_SAVER_HPP
#define NEARGYE_STATE_SAVER_HPP

#include <cstddef>
#include <new>
#include <type_traits>
#if (defined(_MSC_VER) && _MSC_VER >= 1900) || ((defined(__clang__) || defined(__GNUC__)) && __cplusplus >= 201700L)
#include <exception>
#endif

// state_saver throwable settings:
// STATE_SAVER_NO_THROW_CONSTRUCTIBLE requires nothrow constructible action.
// STATE_SAVER_MAY_THROW_RESTORE restore may throw exceptions.
// STATE_SAVER_NO_THROW_RESTORE requires noexcept restore.
// STATE_SAVER_SUPPRESS_THROW_RESTORE exceptions during restore will be suppressed.

// state_saver assignable settings:
// STATE_SAVER_FORCE_MOVE_ASSIGNABLE restore on scope exit will be move assigned.
// STATE_SAVER_FORCE_COPY_ASSIGNABLE restore on scope exit will be copy assigned.

#if !defined(STATE_SAVER_MAY_THROW_RESTORE) && !defined(STATE_SAVER_NO_THROW_RESTORE) && !defined(STATE_SAVER_SUPPRESS_THROW_RESTORE)
#  define STATE_SAVER_MAY_THROW_RESTORE
#elif (defined(STATE_SAVER_MAY_THROW_RESTORE) + defined(STATE_SAVER_NO_THROW_RESTORE) + defined(STATE_SAVER_SUPPRESS_THROW_RESTORE)) > 1
#  error Only one of STATE_SAVER_MAY_THROW_RESTORE and STATE_SAVER_NO_THROW_RESTORE and STATE_SAVER_SUPPRESS_THROW_RESTORE may be defined.
#endif

#if (defined(STATE_SAVER_FORCE_MOVE_ASSIGNABLE) + defined(STATE_SAVER_FORCE_COPY_ASSIGNABLE)) > 1
#  error Only one of STATE_SAVER_FORCE_MOVE_ASSIGNABLE and STATE_SAVER_FORCE_COPY_ASSIGNABLE may be defined.
#endif

#if defined(STATE_SAVER_SUPPRESS_THROW_RESTORE) && (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND))
#  define __STATE_SAVER_NOEXCEPT(...) noexcept
#  define __STATE_SAVER_TRY try {
#  define __STATE_SAVER_CATCH } catch (...) {}
#else
#  define __STATE_SAVER_NOEXCEPT(...) noexcept(__VA_ARGS__)
#  define __STATE_SAVER_TRY
#  define __STATE_SAVER_CATCH
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

class on_exit_policy {
  bool restore_{true};

 public:
  void dismiss() noexcept {
    restore_ = false;
  }

  bool should_restore() const noexcept {
    return restore_;
  }
};

class on_fail_policy {
  int ec_{uncaught_exceptions()};

 public:
  void dismiss() noexcept {
    ec_ = -1;
  }

  bool should_restore() const noexcept {
    return ec_ != -1 && ec_ < uncaught_exceptions();
  }
};

class on_success_policy {
  int ec_{uncaught_exceptions()};

 public:
  void dismiss() noexcept {
    ec_ = -1;
  }

  bool should_restore() const noexcept {
    return ec_ != -1 && ec_ >= uncaught_exceptions();
  }
};

template <typename U, typename P>
class state_saver {
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
                "state_saver requires not const type.");
  static_assert(!std::is_rvalue_reference<U>::value && (std::is_lvalue_reference<U>::value || std::is_same<T, U>::value),
                "state_saver requires lvalue type.");
  static_assert(!std::is_array<T>::value,
                "state_saver requires not array type.");
  static_assert(!std::is_pointer<T>::value,
                "state_saver requires not pointer type.");
  static_assert(!std::is_function<T>::value,
                "state_saver requires not function type.");
  static_assert(std::is_constructible<T, T&>::value,
                "state_saver requires copy constructible.");
  static_assert(std::is_assignable<T&, assignable_t>::value,
                "state_saver requires operator=.");
  static_assert(std::is_same<P, on_exit_policy>::value || std::is_same<P, on_fail_policy>::value || std::is_same<P, on_success_policy>::value,
                "state_saver requires on_exit_policy, on_fail_policy or on_success_policy.");
#if defined(STATE_SAVER_NO_THROW_RESTORE)
  static_assert(std::is_nothrow_assignable<T&, assignable_t>::value,
                "state_saver requires noexcept operator=.");
#endif
#if defined(STATE_SAVER_NO_THROW_CONSTRUCTIBLE)
  static_assert(std::is_nothrow_constructible<T, T&>::value,
                "state_saver requires nothrow constructible.");
#endif

  P policy_;
  T& previous_ref_;
  T previous_value_;

  void* operator new(std::size_t) = delete;
  void operator delete(void*) = delete;

 public:
  state_saver() = delete;
  state_saver(const state_saver&) = delete;
  state_saver(state_saver&&) = delete;
  state_saver& operator=(const state_saver&) = delete;
  state_saver& operator=(state_saver&&) = delete;

  state_saver(T&&) = delete;
  state_saver(const T&) = delete;

  explicit state_saver(T& object) noexcept(std::is_nothrow_constructible<T, T&>::value)
      : policy_{},
        previous_ref_{object},
        previous_value_{object} {}

  void dismiss() noexcept {
    policy_.dismiss();
  }

  template <typename O = T>
  auto restore() __STATE_SAVER_NOEXCEPT(std::is_nothrow_assignable<T&, T&>::value) -> typename std::enable_if<std::is_same<T, O>::value && std::is_assignable<T&, O&>::value>::type {
    static_assert(std::is_assignable<T&, T&>::value, "state_saver::restore requires copy operator=.");
#if defined(STATE_SAVER_NO_THROW_RESTORE)
    static_assert(std::is_nothrow_assignable<T&, T&>::value, "state_saver::restore requires noexcept copy operator=.");
#endif
    __STATE_SAVER_TRY
      previous_ref_ = previous_value_;
    __STATE_SAVER_CATCH
  }

  ~state_saver() __STATE_SAVER_NOEXCEPT(std::is_nothrow_assignable<T&, assignable_t>::value) {
    if (policy_.should_restore()) {
      __STATE_SAVER_TRY
        previous_ref_ = static_cast<assignable_t>(previous_value_);
      __STATE_SAVER_CATCH
    }
  }
};

} // namespace state_saver::detail

template <typename U>
class saver_exit : public detail::state_saver<U, detail::on_exit_policy> {
 public:
  using detail::state_saver<U, detail::on_exit_policy>::state_saver;
};

template <typename U>
class saver_fail : public detail::state_saver<U, detail::on_fail_policy> {
 public:
  using detail::state_saver<U, detail::on_fail_policy>::state_saver;
};

template <typename U>
class saver_succes : public detail::state_saver<U, detail::on_success_policy> {
 public:
  using detail::state_saver<U, detail::on_success_policy>::state_saver;
};

#if defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 201611L
template <typename U>
saver_exit(U&) -> saver_exit<U>;

template <typename U>
saver_fail(U&) -> saver_fail<U>;

template <typename U>
saver_succes(U&) -> saver_succes<U>;
#endif

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

#define __STATE_SAVER_STR_CONCAT_IMPL(s1, s2) s1##s2
#define __STATE_SAVER_STR_CONCAT(s1, s2) __STATE_SAVER_STR_CONCAT_IMPL(s1, s2)

#if defined(__COUNTER__)
#  define __STATE_SAVER_COUNTER __COUNTER__
#elif defined(__LINE__)
#  define __STATE_SAVER_COUNTER __LINE__
#endif

#if __cplusplus >= 201703L || defined(_MSVC_LANG) && _MSVC_LANG >= 201703L
#  define __STATE_SAVER_WITH(s) if (s; true)
#else
#  define __STATE_SAVER_WITH_IMPL(s, i) if (int i = 1) for (s; i; --i)
#  define __STATE_SAVER_WITH(s) __STATE_SAVER_WITH_IMPL(s, __STATE_SAVER_STR_CONCAT(__state_saver_with_internal__object_, __STATE_SAVER_COUNTER))
#endif

// SAVER_EXIT saves the origin variable value and restores on scope exit, undoes any changes that could occure to the object.
#define MAKE_SAVER_EXIT(name, x) ::state_saver::saver_exit<decltype(x)> name{x}
#define SAVER_EXIT(x) ATTR_MAYBE_UNUSED const MAKE_SAVER_EXIT(__STATE_SAVER_STR_CONCAT(__state_saver_exit__object_, __STATE_SAVER_COUNTER), x)
#define WITH_SAVER_EXIT(x) __STATE_SAVER_WITH(SAVER_EXIT(x))

// SAVER_FAIL saves the origin variable value and restores on scope exit when an exception has been thrown before scope exit, undoes any changes that could occure to the object.
#define MAKE_SAVER_FAIL(name, x) ::state_saver::saver_fail<decltype(x)> name{x}
#define SAVER_FAIL(x) ATTR_MAYBE_UNUSED const MAKE_SAVER_FAIL(__STATE_SAVER_STR_CONCAT(__state_saver_fail__object_, __STATE_SAVER_COUNTER), x)
#define WITH_SAVER_FAIL(x) __STATE_SAVER_WITH(SAVER_FAIL(x))

// SAVER_SUCCESS saves the origin variable value and restores on scope exit when no exceptions have been thrown before scope exit, undoes any changes that could occure to the object.
#define MAKE_SAVER_SUCCESS(name, x) ::state_saver::saver_succes<decltype(x)> name{x}
#define SAVER_SUCCESS(x) ATTR_MAYBE_UNUSED const MAKE_SAVER_SUCCESS(__STATE_SAVER_STR_CONCAT(__state_saver_succes__object_, __STATE_SAVER_COUNTER), x)
#define WITH_SAVER_SUCCESS(x) __STATE_SAVER_WITH(SAVER_SUCCESS(x))

#endif // NEARGYE_STATE_SAVER_HPP
