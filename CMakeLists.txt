cmake_minimum_required(VERSION 3.21)
project(Shell C)

set(CMAKE_C_STANDARD 99)

add_executable(Shell src/main.c)

target_include_directories(Shell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
