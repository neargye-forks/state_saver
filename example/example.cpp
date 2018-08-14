// state_saver example
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

#include <state_saver.hpp>

#include <iostream>

void Foo1(int& a) {
  STATE_SAVER(a);
  a = 100;
  std::cout << "Foo1::a = " << a << std::endl;
}

void Foo3(int& a) {
  using namespace state_saver;

#if defined(__cpp_deduction_guides) && __cpp_deduction_guides >= 201611L
  StateSaver state_saver{a};
#else
  StateSaver<decltype(a)> state_saver{a}; // or StateSaver<int> state_saver{a};
#endif

  a = 100;
  std::cout << "Foo3::a = " << a << std::endl;
}

int main() {
  int a = 1;
  std::cout << "main::a = " << a << std::endl;

  Foo1(a);
  std::cout << "main::a = " << a << std::endl;

  auto Foo2 = [&]() {
    MAKE_STATE_SAVER(state_saver, a);
    a = 100;
    std::cout << "Foo2::a = " << a << std::endl;
    state_saver.Restore(/*restore_force =*/ true);
    std::cout << "Foo2::a = " << a << std::endl;
    a = 101;
    state_saver.Dismiss();
  };
  Foo2();
  std::cout << "main::a = " << a << std::endl;

  Foo3(a);
  std::cout << "main::a = " << a << std::endl;

  return 0;
}
