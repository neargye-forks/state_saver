# State Saver C++

```text
  _____ _        _          _____                         _____
 / ____| |      | |        / ____|                       / ____|_     _
| (___ | |_ __ _| |_ ___  | (___   __ ___   _____ _ __  | |   _| |_ _| |_
 \___ \| __/ _` | __/ _ \  \___ \ / _` \ \ / / _ \ '__| | |  |_   _|_   _|
 ____) | || (_| | ||  __/  ____) | (_| |\ V /  __/ |    | |____|_|   |_|
|_____/ \__\__,_|\__\___| |_____/ \__,_| \_/ \___|_|     \_____|
```

Branch | Linux/OSX | Windows | License | Codacy
-------|-----------|---------|---------|-------
master |[![Build Status](https://travis-ci.org/Neargye/state_saver.svg?branch=master)](https://travis-ci.org/Neargye/state_saver)|[![Build status](https://ci.appveyor.com/api/projects/status/64trm7iqd1a9gg6u/branch/master?svg=true)](https://ci.appveyor.com/project/Neargye/state-saver/branch/master)|[![License](https://img.shields.io/github/license/Neargye/state_saver.svg)](LICENSE)|[![Codacy Badge](https://api.codacy.com/project/badge/Grade/d5ef10058bf44e57acc657d106aa2522)](https://www.codacy.com/app/Neargye/state_saver?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Neargye/state_saver&amp;utm_campaign=Badge_Grade)

## What is State Saver?

state_saver_exit - saves the origin variable value and restores on scope exit, undoes any changes that could occure to the object.

state_saver_fail - saves the origin variable value and restores on scope exit when an exception has been thrown before the block's end, undoes any changes that could occure to the object.

state_saver_succes - saves the origin variable value and restores on scope exit when no exceptions have been thrown, undoes any changes that could occure to the object.

## Features

* Simple syntax
* C++11
* Header-only
* Dependency-free

## [Examples](example)

* state_saver_exit
```cpp
void Foo(A& a)
  STATE_SAVER_EXIT(a);
  ... // Alter state from non-const functions.
  ... // Other code.
  // Original state automatically restored on scope exit.
}
```

* state_saver_fail
```cpp
void Foo(A& a)
  STATE_SAVER_FAIL(a);
  ... // Alter state from non-const functions.
  ... // Other code.
  // Original state automatically restored when an exception has been thrown before the block's end.
}
```

* state_saver_succes
```cpp
void Foo(A& a)
  STATE_SAVER_SUCCESS(a);
  ... // Alter state from non-const functions.
  ... // Other code.
  // Original state automatically restored when no exceptions have been thrown
}
```

## Integration

You should add required file [state_saver.hpp](include/state_saver.hpp).

## Compiler compatibility

* GCC
* Clang
* MSVC

## Licensed under the [MIT License](LICENSE)
