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

#include <state_saver.hpp>

#include <stdexcept>

#include "test_case.hpp"

A na;
static_assert(noexcept(state_saver::StateSaver<A>{na}), "");
static_assert(noexcept(state_saver::StateSaver<A>{na}.~StateSaver()), "");

constexpr const int value = -1;
constexpr const int other_value = 1;

TEST_CASE("called on scope leave function") {
  SECTION("StateSaver") {
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
  }

  SECTION("custom StateSaver") {
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
  }
}

TEST_CASE("called on scope leave lambda") {
  SECTION("StateSaver") {
    A a{value};
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

  SECTION("custom StateSaver") {
    A a{value};
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

TEST_CASE("called on exception function") {
  SECTION("StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      STATE_SAVER(a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::exception{};
    };

    REQUIRE_THROWS([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }

  SECTION("custom StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::exception{};
    };

    REQUIRE_THROWS([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("called on exception lambda") {
  SECTION("StateSaver") {
    A a{value};
    const auto SomeLambda = [&]() {
      STATE_SAVER(a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::exception{};
    };

    REQUIRE_THROWS([&]() {
      SomeLambda();
    }());
    REQUIRE(a.i == value);
  }

  SECTION("custom StateSaver") {
    A a{value};
    const auto SomeLambda = [&]() {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::exception{};
    };

    REQUIRE_THROWS([&]() {
      SomeLambda();
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("dismiss before scope leave function") {
  SECTION("custom StateSaver") {
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

TEST_CASE("dismiss before scope leave lambda") {
  SECTION("custom StateSaver") {
    A a{value};
    const auto SomeLambda = [&]() {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Dismiss();
    };

    REQUIRE_NOTHROW([&]() {
      SomeLambda();
    }());
    REQUIRE(a.i == other_value);
  }
}

TEST_CASE("dismiss before exception function") {
  SECTION("custom StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Dismiss();
      throw std::exception{};
    };

    REQUIRE_THROWS([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == other_value);
  }
}

TEST_CASE("dismiss before exception lambda") {
  SECTION("custom StateSaver") {
    A a{value};
    const auto SomeLambda = [&]() {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      state_saver.Dismiss();
      throw std::exception{};
    };

    REQUIRE_THROWS([&]() {
      SomeLambda();
    }());
    REQUIRE(a.i == other_value);
  }
}

TEST_CASE("called on exception, dismiss after exception") {
  SECTION("custom StateSaver") {
    A a{value};
    const auto SomeFunction = [](A& a) {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      throw std::exception{};
      state_saver.Dismiss();
    };

    REQUIRE_THROWS([&]() {
      SomeFunction(a);
    }());
    REQUIRE(a.i == value);
  }
}

TEST_CASE("called on exception, dismiss after exception lambda") {
  SECTION("custom StateSaver") {
    A a{value};
    const auto SomeLambda = [&]() {
      MAKE_STATE_SAVER(state_saver, a);
      a.i = other_value;
      REQUIRE(a.i == other_value);
      REQUIRE(a.i == other_value);
      throw std::exception{};
      state_saver.Dismiss();
    };

    REQUIRE_THROWS([&]() {
      SomeLambda();
    }());
    REQUIRE(a.i == value);
  }
}
