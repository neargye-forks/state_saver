// state_saver test
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

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#define REQUIRE_NOTHROW_IF(is_nothrow, ...) \
  if (is_nothrow) {                         \
    REQUIRE_NOTHROW(__VA_ARGS__);           \
  } else {                                  \
    REQUIRE_THROWS(__VA_ARGS__);            \
  }

#include <state_saver.hpp>

using namespace state_saver;

#include <stdexcept>

#include "test_case.hpp"

constexpr int value = -1;
constexpr int other_value = 1;

static constexpr bool IsNothrowCopyAssignable = std::is_nothrow_assignable<A&, A&>::value;

TEST_CASE("compilation") {
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
  static_assert(noexcept(StateSaver<decltype(va)>{va}), "");
  static_assert(noexcept(StateSaver<decltype(va)>{va}.~StateSaver()), "");

  A& ra = va;
  static_assert(noexcept(StateSaver<decltype(ra)>{ra}), "");
  static_assert(noexcept(StateSaver<decltype(ra)>{ra}.~StateSaver()), "");

#if defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 201611L
  static_assert(noexcept(StateSaver{va}), "");
  static_assert(noexcept(StateSaver{va}.~StateSaver()), "");

  static_assert(noexcept(StateSaver{ra}), "");
  static_assert(noexcept(StateSaver{ra}.~StateSaver()), "");
#endif
}

TEST_CASE("called on scope leave") {
  SECTION("StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      StateSaver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);

    const auto SomeLambda = [&]() {
      StateSaver<decltype(a)> state_saver{a};
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
    const auto SomeFunction = [](A& a) {
      STATE_SAVER(a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      SomeFunction(a);
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
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      SomeFunction(a);
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

TEST_CASE("called on error") {
  SECTION("StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      StateSaver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }

  SECTION("STATE_SAVER") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      STATE_SAVER(a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("dismiss before scope leave") {
  SECTION("StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      StateSaver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Dismiss();
    };

    REQUIRE_NOTHROW([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == other_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Dismiss();
    };

    REQUIRE_NOTHROW([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == other_value);
  }
}

TEST_CASE("dismiss before error") {
  SECTION("StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      StateSaver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Dismiss();
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == other_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Dismiss();
      throw std::runtime_error{"error"};
    };

    REQUIRE_THROWS([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == other_value);
  }
}

TEST_CASE("called on error, dismiss after error") {
  SECTION("StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      StateSaver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::runtime_error{"error"};
      state_saver.Dismiss();
    };

    REQUIRE_THROWS([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::runtime_error{"error"};
      state_saver.Dismiss();
    };

    REQUIRE_THROWS([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("restore") {
  SECTION("StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      StateSaver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Restore(false);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(IsNothrowCopyAssignable, [&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Restore(false);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(IsNothrowCopyAssignable, [&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("restore force") {
  SECTION("StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      StateSaver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Restore(true);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(IsNothrowCopyAssignable, [&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Restore(true);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(IsNothrowCopyAssignable, [&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("dismiss, restore") {
  SECTION("StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      StateSaver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Dismiss();
      state_saver.Restore(false);
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == other_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Dismiss();
      state_saver.Restore(false);
      REQUIRE(a.i == other_value);
    };

    REQUIRE_NOTHROW([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == other_value);
  }
}

TEST_CASE("dismiss, restore force") {
  SECTION("StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      StateSaver<decltype(a)> state_saver{a};
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Dismiss();
      state_saver.Restore(true);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(IsNothrowCopyAssignable, [&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == other_value);
  }

  SECTION("MAKE_STATE_SAVER") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Dismiss();
      state_saver.Restore(true);
      REQUIRE(a.i == value);
      a.i = other_value;
    };

    REQUIRE_NOTHROW_IF(IsNothrowCopyAssignable, [&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == other_value);
  }
}
