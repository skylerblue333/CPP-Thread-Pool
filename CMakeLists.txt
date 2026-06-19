cmake_minimum_required(VERSION 3.10)
project(CPP-Thread-Pool)

set(CMAKE_CXX_STANDARD 17)

add_executable(main src/main.cpp)
add_executable(test_main tests/test_main.cpp)
