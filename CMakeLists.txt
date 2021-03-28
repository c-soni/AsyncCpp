cmake_minimum_required(VERSION 3.19)

project(AsyncCpp)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# add the executable
add_executable(AsyncCpp src/main.cpp)

target_compile_options(AsyncCpp
    PUBLIC -O3
    PUBLIC -pthread
    PUBLIC -Werror
    PUBLIC -Wall
    PUBLIC -Wextra
    PUBLIC -Wpedantic
    PUBLIC -Wshadow
    PUBLIC -Wconversion)

target_include_directories(AsyncCpp
    PRIVATE inc)

target_precompile_headers(AsyncCpp
    PRIVATE inc/pch.h)

target_link_libraries(AsyncCpp
    PRIVATE Threads::Threads)