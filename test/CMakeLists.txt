include_directories(3rdparty/Catch2)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(name_test ${PROJECT_NAME}_test)
add_executable(${name_test} test.cpp)
add_test(NAME ${name_test} COMMAND ${name_test})
