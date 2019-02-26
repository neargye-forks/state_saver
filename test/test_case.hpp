// state_saver test_case
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

#include <utility>
#include <stdexcept>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <state_saver.hpp>
using namespace yal;

#if !defined(CASE_NAME)
#  define CASE_NAME_STR_(x) #x
#  define CASE_NAME_STR(x) CASE_NAME_STR_(x)
#  define CASE_NAME CASE_NAME_STR(CASE_NUMBER)
#endif

#if !defined(test_class)
#  define test_class STATE_SAVER_STR_CONCAT(test_class_, CASE_NUMBER)
#endif

class test_class {
 public:
  int i = 0;

  explicit test_class(int i) : i{i} {}
  test_class() = default;
  ~test_class() = default;
  test_class(test_class&&) = default;

#if CASE_NUMBER == 1
  test_class(const test_class&) = default;
  test_class& operator=(const test_class&) = default;
  test_class& operator=(test_class&&) = default;
#elif CASE_NUMBER == 2
  test_class(const test_class&) = default;
  test_class& operator=(const test_class&) = default;
  test_class& operator=(test_class&&) = delete;
#elif CASE_NUMBER == 3
  test_class(const test_class&) = default;
  test_class& operator=(test_class&) = default;
  test_class& operator=(test_class&&) = default;
#elif CASE_NUMBER == 4
  test_class(const test_class&) = default;
  test_class& operator=(test_class&) = default;
  test_class& operator=(test_class&&) = delete;
#elif CASE_NUMBER == 5
  test_class(test_class&) = default;
  test_class& operator=(const test_class&) = default;
  test_class& operator=(test_class&&) = default;
#elif CASE_NUMBER == 6
  test_class(test_class&) = default;
  test_class& operator=(const test_class&) = default;
  test_class& operator=(test_class&&) = delete;
#elif CASE_NUMBER == 7
  test_class(test_class&) = default;
  test_class& operator=(test_class&) = default;
  test_class& operator=(test_class&&) = default;
#elif CASE_NUMBER == 8
  test_class(test_class&) = default;
  test_class& operator=(test_class&) = default;
  test_class& operator=(test_class&&) = delete;
#elif CASE_NUMBER == 9
  test_class(const test_class&) = default;
  test_class& operator=(const test_class&) noexcept(false) {
    throw std::runtime_error{"operator=(const test_class" CASE_NAME "&) throw."};
  }
  test_class& operator=(test_class&&) = default;
#elif CASE_NUMBER == 10
  test_class(const test_class&) = default;
  test_class& operator=(const test_class&) = default;
  test_class& operator=(test_class&&) noexcept(false) {
    throw std::runtime_error{"operator=(test_class" CASE_NAME "&&) throw."};
  }
#endif
};

#if !defined(test_value)
#  define test_value 1
#endif

#if !defined(other_test_value)
#  define other_test_value -1
#endif

#if !defined(is_test_class_nothrow_copy_assignable_v)
#  define is_test_class_nothrow_copy_assignable_v std::is_nothrow_assignable<test_class&, test_class&>::value
#endif

#if !defined(REQUIRE_NOTHROW_IF)
#  define REQUIRE_NOTHROW_IF(is_nothrow, ...) \
    if (is_nothrow) {                         \
      REQUIRE_NOTHROW(__VA_ARGS__);           \
    } else {                                  \
      REQUIRE_THROWS(__VA_ARGS__);            \
    }
#endif

TEST_CASE("Static test " CASE_NAME) {
#if CASE_NUMBER == 1
  static_assert(std::is_constructible<test_class, test_class&>::value, "");
  static_assert(std::is_nothrow_constructible<test_class, test_class&>::value, "");

  static_assert(std::is_assignable<test_class&, test_class>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class>::value, "");

  static_assert(std::is_assignable<test_class&, test_class&>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class&>::value, "");
#elif CASE_NUMBER == 2
  static_assert(std::is_constructible<test_class, test_class&>::value, "");
  static_assert(std::is_nothrow_constructible<test_class, test_class&>::value, "");

  static_assert(!std::is_assignable<test_class&, test_class>::value, "");
  static_assert(!std::is_nothrow_assignable<test_class&, test_class>::value, "");

  static_assert(std::is_assignable<test_class&, test_class&>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class&>::value, "");
#elif CASE_NUMBER == 3
  static_assert(std::is_constructible<test_class, test_class&>::value, "");
  static_assert(std::is_nothrow_constructible<test_class, test_class&>::value, "");

  static_assert(std::is_assignable<test_class&, test_class>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class>::value, "");

  static_assert(std::is_assignable<test_class&, test_class&>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class&>::value, "");
#elif CASE_NUMBER == 4
  static_assert(std::is_constructible<test_class, test_class&>::value, "");
  static_assert(std::is_nothrow_constructible<test_class, test_class&>::value, "");

  static_assert(!std::is_assignable<test_class&, test_class>::value, "");
  static_assert(!std::is_nothrow_assignable<test_class&, test_class>::value, "");

  static_assert(std::is_assignable<test_class&, test_class&>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class&>::value, "");
#elif CASE_NUMBER == 5
  static_assert(std::is_constructible<test_class, test_class&>::value, "");
  static_assert(std::is_nothrow_constructible<test_class, test_class&>::value, "");

  static_assert(std::is_assignable<test_class&, test_class>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class>::value, "");

  static_assert(std::is_assignable<test_class&, test_class&>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class&>::value, "");
#elif CASE_NUMBER == 6
  static_assert(std::is_constructible<test_class, test_class&>::value, "");
  static_assert(std::is_nothrow_constructible<test_class, test_class&>::value, "");

  static_assert(!std::is_assignable<test_class&, test_class>::value, "");
  static_assert(!std::is_nothrow_assignable<test_class&, test_class>::value, "");

  static_assert(std::is_assignable<test_class&, test_class&>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class&>::value, "");
#elif CASE_NUMBER == 7
  static_assert(std::is_constructible<test_class, test_class&>::value, "");
  static_assert(std::is_nothrow_constructible<test_class, test_class&>::value, "");

  static_assert(std::is_assignable<test_class&, test_class>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class>::value, "");

  static_assert(std::is_assignable<test_class&, test_class&>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class&>::value, "");
#elif CASE_NUMBER == 8
  static_assert(std::is_constructible<test_class, test_class&>::value, "");
  static_assert(std::is_nothrow_constructible<test_class, test_class&>::value, "");

  static_assert(!std::is_assignable<test_class&, test_class>::value, "");
  static_assert(!std::is_nothrow_assignable<test_class&, test_class>::value, "");

  static_assert(std::is_assignable<test_class&, test_class&>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class&>::value, "");

#elif CASE_NUMBER == 9
  static_assert(std::is_constructible<test_class, test_class&>::value, "");
  static_assert(std::is_nothrow_constructible<test_class, test_class&>::value, "");

  static_assert(std::is_assignable<test_class&, test_class>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class>::value, "");

  static_assert(std::is_assignable<test_class&, test_class&>::value, "");
  static_assert(!std::is_nothrow_assignable<test_class&, test_class&>::value, "");
#elif CASE_NUMBER == 10
  static_assert(std::is_constructible<test_class, test_class&>::value, "");
  static_assert(std::is_nothrow_constructible<test_class, test_class&>::value, "");

  static_assert(std::is_assignable<test_class&, test_class>::value, "");
  static_assert(!std::is_nothrow_assignable<test_class&, test_class>::value, "");

  static_assert(std::is_assignable<test_class&, test_class&>::value, "");
  static_assert(std::is_nothrow_assignable<test_class&, test_class&>::value, "");
#endif

  test_class va;
  static_assert(noexcept(state_saver<decltype(va)>{va}), "");
  static_assert(noexcept(state_saver<decltype(va)>{va}.~state_saver()), "");

  test_class& ra = va;
  static_assert(noexcept(state_saver<decltype(ra)>{ra}), "");
  static_assert(noexcept(state_saver<decltype(ra)>{ra}.~state_saver()), "");

#if defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 201611L
  static_assert(noexcept(state_saver{va}), "");
  static_assert(noexcept(state_saver{va}.~state_saver()), "");

  static_assert(noexcept(state_saver{ra}), "");
  static_assert(noexcept(state_saver{ra}.~state_saver()), "");
#endif
}

TEST_CASE("Called on scope leave " CASE_NAME) {
  SECTION("state_saver") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);

    const auto SomeLambda = [&]() {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
    };

    REQUIRE_NOTHROW([&]() {
      SomeLambda();
    }());
    REQUIRE(a.i == test_value);
  }

  SECTION("STATE_SAVER") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      STATE_SAVER(a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);

    const auto SomeLambda = [&]() {
      STATE_SAVER(a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
    };

    REQUIRE_NOTHROW([&]() {
      SomeLambda();
    }());
    REQUIRE(a.i == test_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);

    const auto SomeLambda = [&]() {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
    };

    REQUIRE_NOTHROW([&]() {
      SomeLambda();
    }());
    REQUIRE(a.i == test_value);
  }
}

TEST_CASE("Called on error " CASE_NAME) {
  SECTION("state_saver") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);
  }

  SECTION("STATE_SAVER") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      STATE_SAVER(a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);
  }
}

TEST_CASE("Dismiss before scope leave " CASE_NAME) {
  SECTION("state_saver") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.dismiss();
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_test_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.dismiss();
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_test_value);
  }
}

TEST_CASE("Dismiss before error " CASE_NAME) {
  SECTION("state_saver") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.dismiss();
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_test_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.dismiss();
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_test_value);
  }
}

TEST_CASE("Called on error, dismiss after error " CASE_NAME) {
  SECTION("state_saver") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      throw std::runtime_error{"error"};
      state_saver.dismiss();
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      throw std::runtime_error{"error"};
      state_saver.dismiss();
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);
  }
}

TEST_CASE("Restore " CASE_NAME) {
  SECTION("state_saver") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.restore(false);
      REQUIRE(a.i == test_value);
      a.i = other_test_value;
    };

    REQUIRE_NOTHROW_IF(is_test_class_nothrow_copy_assignable_v, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.restore(false);
      REQUIRE(a.i == test_value);
      a.i = other_test_value;
    };

    REQUIRE_NOTHROW_IF(is_test_class_nothrow_copy_assignable_v, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);
  }
}

TEST_CASE("Restore force " CASE_NAME) {
  SECTION("state_saver") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.restore(true);
      REQUIRE(a.i == test_value);
      a.i = other_test_value;
    };

    REQUIRE_NOTHROW_IF(is_test_class_nothrow_copy_assignable_v, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.restore(true);
      REQUIRE(a.i == test_value);
      a.i = other_test_value;
    };

    REQUIRE_NOTHROW_IF(is_test_class_nothrow_copy_assignable_v, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == test_value);
  }
}

TEST_CASE("Dismiss, restore " CASE_NAME) {
  SECTION("state_saver") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.dismiss();
      state_saver.restore(false);
      REQUIRE(a.i == other_test_value);
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_test_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.dismiss();
      state_saver.restore(false);
      REQUIRE(a.i == other_test_value);
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_test_value);
  }
}

TEST_CASE("Dismiss, restore force " CASE_NAME) {
  SECTION("state_saver") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.dismiss();
      state_saver.restore(true);
      REQUIRE(a.i == test_value);
      a.i = other_test_value;
    };

    REQUIRE_NOTHROW_IF(is_test_class_nothrow_copy_assignable_v, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_test_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    test_class a{test_value};
    const auto some_function = [](test_class& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_test_value;
      REQUIRE(a.i == other_test_value);
      state_saver.dismiss();
      state_saver.restore(true);
      REQUIRE(a.i == test_value);
      a.i = other_test_value;
    };

    REQUIRE_NOTHROW_IF(is_test_class_nothrow_copy_assignable_v, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_test_value);
  }
}
