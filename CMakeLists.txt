cmake_minimum_required(VERSION 3.6)

project(state_saver VERSION "0.4.6" LANGUAGES CXX)

option(STATE_SAVER_OPT_BUILD_EXAMPLES "Build state_saver examples" ON)
option(STATE_SAVER_OPT_BUILD_TESTS "Build and perform state_saver tests" ON)

if(STATE_SAVER_OPT_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(STATE_SAVER_OPT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
