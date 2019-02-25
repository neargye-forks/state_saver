// state_saver test
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

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#define REQUIRE_NOTHROW_IF(is_nothrow, ...) \
  if (is_nothrow) {                         \
    REQUIRE_NOTHROW(__VA_ARGS__);           \
  } else {                                  \
    REQUIRE_THROWS(__VA_ARGS__);            \
  }

#include <state_saver.hpp>

using namespace yal;

#include <stdexcept>

#include "test_case.hpp"

constexpr int value = -1;
constexpr int other_value = 1;

static constexpr bool is_nothrow_copy_assignable = std::is_nothrow_assignable<A&, A&>::value;

TEST_CASE("Static test") {
#if defined(TEST_CASE_1)
  static_assert(std::is_constructible<A, A&>::value, "");
  static_assert(std::is_nothrow_constructible<A, A&>::value, "");

  static_assert(std::is_assignable<A&, A>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A>::value, "");

  static_assert(std::is_assignable<A&, A&>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A&>::value, "");
#elif defined(TEST_CASE_2)
  static_assert(std::is_constructible<A, A&>::value, "");
  static_assert(std::is_nothrow_constructible<A, A&>::value, "");

  static_assert(!std::is_assignable<A&, A>::value, "");
  static_assert(!std::is_nothrow_assignable<A&, A>::value, "");

  static_assert(std::is_assignable<A&, A&>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A&>::value, "");
#elif defined(TEST_CASE_3)
  static_assert(std::is_constructible<A, A&>::value, "");
  static_assert(std::is_nothrow_constructible<A, A&>::value, "");

  static_assert(std::is_assignable<A&, A>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A>::value, "");

  static_assert(std::is_assignable<A&, A&>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A&>::value, "");
#elif defined(TEST_CASE_4)
  static_assert(std::is_constructible<A, A&>::value, "");
  static_assert(std::is_nothrow_constructible<A, A&>::value, "");

  static_assert(!std::is_assignable<A&, A>::value, "");
  static_assert(!std::is_nothrow_assignable<A&, A>::value, "");

  static_assert(std::is_assignable<A&, A&>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A&>::value, "");
#elif defined(TEST_CASE_5)
  static_assert(std::is_constructible<A, A&>::value, "");
  static_assert(std::is_nothrow_constructible<A, A&>::value, "");

  static_assert(std::is_assignable<A&, A>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A>::value, "");

  static_assert(std::is_assignable<A&, A&>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A&>::value, "");
#elif defined(TEST_CASE_6)
  static_assert(std::is_constructible<A, A&>::value, "");
  static_assert(std::is_nothrow_constructible<A, A&>::value, "");

  static_assert(!std::is_assignable<A&, A>::value, "");
  static_assert(!std::is_nothrow_assignable<A&, A>::value, "");

  static_assert(std::is_assignable<A&, A&>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A&>::value, "");
#elif defined(TEST_CASE_7)
  static_assert(std::is_constructible<A, A&>::value, "");
  static_assert(std::is_nothrow_constructible<A, A&>::value, "");

  static_assert(std::is_assignable<A&, A>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A>::value, "");

  static_assert(std::is_assignable<A&, A&>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A&>::value, "");
#elif defined(TEST_CASE_8)
  static_assert(std::is_constructible<A, A&>::value, "");
  static_assert(std::is_nothrow_constructible<A, A&>::value, "");

  static_assert(!std::is_assignable<A&, A>::value, "");
  static_assert(!std::is_nothrow_assignable<A&, A>::value, "");

  static_assert(std::is_assignable<A&, A&>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A&>::value, "");

#elif defined(TEST_CASE_9)
  static_assert(std::is_constructible<A, A&>::value, "");
  static_assert(std::is_nothrow_constructible<A, A&>::value, "");

  static_assert(std::is_assignable<A&, A>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A>::value, "");

  static_assert(std::is_assignable<A&, A&>::value, "");
  static_assert(!std::is_nothrow_assignable<A&, A&>::value, "");
#elif defined(TEST_CASE_10)
  static_assert(std::is_constructible<A, A&>::value, "");
  static_assert(std::is_nothrow_constructible<A, A&>::value, "");

  static_assert(std::is_assignable<A&, A>::value, "");
  static_assert(!std::is_nothrow_assignable<A&, A>::value, "");

  static_assert(std::is_assignable<A&, A&>::value, "");
  static_assert(std::is_nothrow_assignable<A&, A&>::value, "");
#endif

  A va;
  static_assert(noexcept(state_saver<decltype(va)>{va}), "");
  static_assert(noexcept(state_saver<decltype(va)>{va}.~state_saver()), "");

  A& ra = va;
  static_assert(noexcept(state_saver<decltype(ra)>{ra}), "");
  static_assert(noexcept(state_saver<decltype(ra)>{ra}.~state_saver()), "");

#if defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 201611L
  static_assert(noexcept(state_saver{va}), "");
  static_assert(noexcept(state_saver{va}.~state_saver()), "");

  static_assert(noexcept(state_saver{ra}), "");
  static_assert(noexcept(state_saver{ra}.~state_saver()), "");
#endif
}

TEST_CASE("Called on scope leave") {
  SECTION("state_saver") {
    A a{value};
    const auto some_function = [](A& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);

    const auto SomeLambda = [&]() {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      SomeLambda();
    }());
    REQUIRE(a.i == value);
  }

  SECTION("STATE_SAVER") {
    A a{value};
    const auto some_function = [](A& a) {
      STATE_SAVER(a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);

    const auto SomeLambda = [&]() {
      STATE_SAVER(a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      SomeLambda();
    }());
    REQUIRE(a.i == value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto some_function = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);

    const auto SomeLambda = [&]() {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      SomeLambda();
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("Called on error") {
  SECTION("state_saver") {
    A a{value};
    const auto some_function = [](A& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);
  }

  SECTION("STATE_SAVER") {
    A a{value};
    const auto some_function = [](A& a) {
      STATE_SAVER(a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto some_function = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("Dismiss before scope leave") {
  SECTION("state_saver") {
    A a{value};
    const auto some_function = [](A& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.dismiss();
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto some_function = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.dismiss();
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_value);
  }
}

TEST_CASE("Dismiss before error") {
  SECTION("state_saver") {
    A a{value};
    const auto some_function = [](A& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.dismiss();
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto some_function = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.dismiss();
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_value);
  }
}

TEST_CASE("Called on error, dismiss after error") {
  SECTION("state_saver") {
    A a{value};
    const auto some_function = [](A& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::runtime_error{"error"};
      state_saver.dismiss();
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto some_function = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::runtime_error{"error"};
      state_saver.dismiss();
    };

    REQUIRE_THROWS([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("Restore") {
  SECTION("state_saver") {
    A a{value};
    const auto some_function = [](A& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.restore(false);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(is_nothrow_copy_assignable, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto some_function = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.restore(false);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(is_nothrow_copy_assignable, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("Restore force") {
  SECTION("state_saver") {
    A a{value};
    const auto some_function = [](A& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.restore(true);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(is_nothrow_copy_assignable, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto some_function = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.restore(true);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(is_nothrow_copy_assignable, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("Dismiss, restore") {
  SECTION("state_saver") {
    A a{value};
    const auto some_function = [](A& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.dismiss();
      state_saver.restore(false);
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto some_function = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.dismiss();
      state_saver.restore(false);
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_value);
  }
}

TEST_CASE("Dismiss, restore force") {
  SECTION("state_saver") {
    A a{value};
    const auto some_function = [](A& a) {
      state_saver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.dismiss();
      state_saver.restore(true);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(is_nothrow_copy_assignable, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto some_function = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.dismiss();
      state_saver.restore(true);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(is_nothrow_copy_assignable, [&]() {
      some_function(a);
    }());
    REQUIRE(a.i == other_value);
  }
}
