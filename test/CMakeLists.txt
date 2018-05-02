include_directories(3rdparty/Catch2)
include_directories(${CMAKE_SOURCE_DIR}/include)

FOREACH(I RANGE 1 10)
    add_executable(${PROJECT_NAME}_test_case_${I} test.cpp)
    add_test(NAME ${PROJECT_NAME}_test_case_${I} COMMAND ${PROJECT_NAME}_test_case_${I})
    target_compile_definitions(${PROJECT_NAME}_test_case_${I} PUBLIC TEST_CASE_${I})
ENDFOREACH()
