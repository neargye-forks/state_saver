```text
  _____ _        _          _____                         _____
 / ____| |      | |        / ____|                       / ____|_     _
| (___ | |_ __ _| |_ ___  | (___   __ ___   _____ _ __  | |   _| |_ _| |_
 \___ \| __/ _` | __/ _ \  \___ \ / _` \ \ / / _ \ '__| | |  |_   _|_   _|
 ____) | || (_| | ||  __/  ____) | (_| |\ V /  __/ |    | |____|_|   |_|
|_____/ \__\__,_|\__\___| |_____/ \__,_| \_/ \___|_|     \_____|
```

[![Github Releases](https://img.shields.io/github/release/Neargye/state_saver.svg)](https://github.com/Neargye/state_saver/releases)
[![License](https://img.shields.io/github/license/Neargye/state_saver.svg)](LICENSE)

# State Saver C++

Sometimes a certain value has to change only for a limited scope. This class wrapper saves a copy of the current state of some object, and resets the object’s state at destruction time, undoing any change the object may have gone through.

* saver_exit - saves the original variable value and restores on scope exit.

* saver_fail - saves the original variable value and restores on scope exit when an exception has been thrown.

* saver_success - saves the original variable value and restores on scope exit when no exceptions have been thrown.

## Features

* C++11
* Header-only
* Dependency-free

## [Examples](example)

* [State Saver on exit](example/state_saver_exit_example.cpp)

  ```cpp
  void Foo(A& a)
    SAVER_EXIT(a);
    ... // Alter state from non-const functions.
    ... // Other code.
    // Original state automatically restored on scope exit.
  }
  ```

* [State Saver on fail](example/state_saver_fail_example.cpp)

  ```cpp
  void Foo(A& a)
    SAVER_FAIL(a);
    ... // Alter state from non-const functions.
    ... // Other code.
    // Original state automatically restored when an exception has been thrown.
  }
  ```

* [State Saver on success](example/state_saver_success_example.cpp)

  ```cpp
  void Foo(A& a)
    SAVER_SUCCESS(a);
    ... // Alter state from non-const functions.
    ... // Other code.
    // Original state automatically restored when no exceptions have been thrown.
  }
  ```

## Synopsis

### Reference

#### saver_exit

* `saver_exit<decltype(object)> state_saver{object};` - creation saver_exit for the object.
* `SAVER_EXIT{object};` - macro for creating saver_exit for the object.
* `MAKE_SAVER_EXIT(name) {object};` - macro for creating named saver_exit for the object.
* `WITH_SAVER_EXIT(object) {/*...*/};` - macro for creating scope with saver_exit for the object.

#### saver_fail

* `saver_fail<decltype(object)> state_saver{object};` - creation saver_fail for the object.
* `SAVER_FAIL{object};` - macro for creating saver_fail for the object.
* `MAKE_SAVER_FAIL(name) {object};` - macro for creating scope with saver_fail for the object.
* `WITH_SAVER_FAIL(object) {/*...*/};` - macro for creating scope with saver_fail for the object.

#### saver_success

* `saver_success<decltype(object)> state_saver{object};` - creation saver_success for the object.
* `SAVER_SUCCESS{object};` - macro for creating saver_success for the object.
* `MAKE_SAVER_SUCCESS(name) {object};` - macro for creating scope with saver_success for the object.
* `WITH_SAVER_SUCCESS(object) {/*...*/};` - macro for creating scope with saver_success for the object.

### Interface of state_saver

saver_exit, saver_fail, saver_success implement state_saver interface.

* constructor `state_saver(T& object)` - construct state_saver with saved object.

* `dismiss()` - dismiss restore on scope exit.

* `restore()` - resets the object’s state. Requirements copy operator =.

### Requirements to saved object

* POD or object semantic (cannot be reference, function, ...).
* Copy constructor.
* operator= (no-throw one preferred).

### Configuration macros

#### Throwable settings

* `STATE_SAVER_NO_THROW_CONSTRUCTIBLE` define this to require nothrow constructible.

* `STATE_SAVER_MAY_THROW_RESTORE` define this to restore may throw exceptions.

* `STATE_SAVER_NO_THROW_RESTORE` define this to require noexcept restore.

* `STATE_SAVER_SUPPRESS_THROW_RESTORE` define this to exceptions during restore will be suppressed.

* By default using `STATE_SAVER_MAY_THROW_RESTORE`.

* `STATE_SAVER_CATCH_HANDLER` define this to add exceptions handler. If `STATE_SAVER_SUPPRESS_THROW_RESTORE` is not defined, it will do nothing.

#### Assignable settings

* `STATE_SAVER_FORCE_MOVE_ASSIGNABLE` define this to restore on scope exit will be move assigned.

* `STATE_SAVER_FORCE_COPY_ASSIGNABLE` define this to restore on scope exit will be copy assigned.

* By default using move assignable if noexcept.

## Integration

You should add required file [state_saver.hpp](include/state_saver.hpp).

## Compiler compatibility

* Clang/LLVM >= 5
* MSVC++ >= 14.0 / Visual Studio >= 2015
* Xcode >= 9
* GCC >= 5

## Licensed under the [MIT License](LICENSE)
