include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES example.cpp ${CMAKE_SOURCE_DIR}/include/state_saver.hpp)

add_executable(example ${SOURCES})

if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    set(CMAKE_VERBOSE_MAKEFILE ON)
    target_compile_options(example PRIVATE -Wall -Wextra -pedantic-errors)
    target_compile_options(example PRIVATE -std=c++11)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(example PRIVATE /W4)
    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.11)
        target_compile_options(example PRIVATE /permissive-)
    endif()
endif()
