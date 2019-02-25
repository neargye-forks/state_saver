cmake_minimum_required(VERSION 3.10)

include(GNUInstallDirs)

project(state_saver LANGUAGES CXX)
set(${PROJECT_NAME}_version "0.3.0")
set(package_name ${PROJECT_NAME})
set(include_source_dir "${CMAKE_SOURCE_DIR}/include")

option(STATE_SAVER_OPT_BUILD_EXAMPLES "Build state_saver examples" ON)
option(STATE_SAVER_OPT_BUILD_TESTS "Build and perform state_saver tests" ON)

add_library(${package_name} INTERFACE)
target_include_directories(${package_name} INTERFACE "$<BUILD_INTERFACE:${include_source_dir}>")

if(STATE_SAVER_OPT_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(STATE_SAVER_OPT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
