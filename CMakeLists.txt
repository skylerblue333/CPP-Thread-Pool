cmake_minimum_required(VERSION 3.20)
project(sky_thread_pool VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sky_thread_pool INTERFACE)
target_include_directories(sky_thread_pool INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(sky_thread_pool INTERFACE cxx_std_20)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sky_thread_pool INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(sky-thread-pool-demo src/main.cpp)
target_link_libraries(sky-thread-pool-demo PRIVATE sky_thread_pool)

include(CTest)
if(BUILD_TESTING)
    add_executable(sky-thread-pool-tests tests/test_main.cpp)
    target_link_libraries(sky-thread-pool-tests PRIVATE sky_thread_pool)
    add_test(NAME sky-thread-pool-tests COMMAND sky-thread-pool-tests)
endif()
