include(CheckCXXCompilerFlag)

if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    set(CMAKE_VERBOSE_MAKEFILE ON)
    set(OPTIONS -Wall -Wextra -pedantic-errors -Werror -std=c++11)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(OPTIONS /W4 /WX)
    check_cxx_compiler_flag(/permissive HAS_PERMISSIVE_FLAG)
    if(HAS_PERMISSIVE_FLAG)
        set(OPTIONS ${OPTIONS} /permissive-)
    endif()
    set(OPTIONS ${OPTIONS} /wd4702) # Disable warning C4702: unreachable code
endif()

add_executable(state_saver_exit_example
                    state_saver_exit_example.cpp
                    ${CMAKE_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME}.hpp)
target_compile_options(state_saver_exit_example PRIVATE ${OPTIONS})
target_link_libraries(state_saver_exit_example PRIVATE ${CMAKE_PROJECT_NAME})

add_executable(state_saver_fail_example
                    state_saver_fail_example.cpp
                    ${CMAKE_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME}.hpp)
target_compile_options(state_saver_fail_example PRIVATE ${OPTIONS})
target_link_libraries(state_saver_fail_example PRIVATE ${CMAKE_PROJECT_NAME})

add_executable(state_saver_succes_example
                    state_saver_succes_example.cpp
                    ${CMAKE_SOURCE_DIR}/include/${CMAKE_PROJECT_NAME}.hpp)
target_compile_options(state_saver_succes_example PRIVATE ${OPTIONS})
target_link_libraries(state_saver_succes_example PRIVATE ${CMAKE_PROJECT_NAME})
