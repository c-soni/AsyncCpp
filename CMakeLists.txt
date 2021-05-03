cmake_minimum_required(VERSION 3.19)

project(AsyncCpp)

include(cmake/SetCompilerWarnings.cmake)

# add the executable
add_executable(AsyncCpp src/main.cpp)
target_include_directories(AsyncCpp
    PRIVATE inc)
target_precompile_headers(AsyncCpp
    PRIVATE inc/pch.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
set_project_warnings(project_options)
target_link_libraries(AsyncCpp PRIVATE project_options Threads::Threads)
