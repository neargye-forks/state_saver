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

#include <string>

int global_int{-1};
TEST_CASE("basic type") {
  SECTION("function") {
    const auto SomeMethod = [](int& a) {
      STATE_SAVER(a);
      a = 100;
      REQUIRE(a == 100);
      REQUIRE(::global_int == 100);
    };
    SomeMethod(::global_int);
    REQUIRE(::global_int == -1);
  }

  SECTION("lambda") {
    const auto SomeMethod = [&]() {
      STATE_SAVER(global_int);
      global_int = 100;
      REQUIRE(global_int == 100);
      REQUIRE(::global_int == 100);
    };
    SomeMethod();
    REQUIRE(::global_int == -1);
  }
}

struct A {
  int i = 0;
  A(int i) : i{i} {}
  A() = default;
  A(const A&) = default;
  A(A&&) = default;
  A& operator=(const A&) = default;
  A& operator=(A&&) = default;
  ~A() = default;
};

A global_a{-1};
TEST_CASE("struct") {
  SECTION("function") {
    const auto SomeMethod = [](A& a) {
      STATE_SAVER(a);
      a.i = 100;
      REQUIRE(a.i == 100);
      REQUIRE(::global_a.i == 100);
    };
    SomeMethod(::global_a);
    REQUIRE(::global_a.i == -1);
  }

  SECTION("lambda") {
    const auto SomeMethod = [&]() {
      STATE_SAVER(global_a);
      global_a.i = 100;
      REQUIRE(global_a.i == 100);
      REQUIRE(::global_a.i == 100);
    };
    SomeMethod();
    REQUIRE(::global_a.i == -1);
  }
}

std::string global_str{"test"};
TEST_CASE("std") {
  SECTION("function") {
    const auto SomeMethod = [](std::string& s) {
      STATE_SAVER(s);
      s[0] = 'o';
      REQUIRE(s == "oest");
      REQUIRE(::global_str == "oest");
    };
    SomeMethod(::global_str);
    REQUIRE(::global_str == "test");
  }

  SECTION("lambda") {
    const auto SomeMethod = [&]() {
      STATE_SAVER(global_str);
      global_str[0] = 'o';
      REQUIRE(global_str == "oest");
      REQUIRE(::global_str == "oest");
    };
    SomeMethod();
    REQUIRE(::global_str == "test");
  }
}
