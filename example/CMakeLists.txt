include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}_example
        ${CMAKE_SOURCE_DIR}/include/state_saver.hpp
        example.cpp)
