cmake_minimum_required(VERSION 3.10)
project(CppPlayGround)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -gdwarf-4 -fno-omit-frame-pointer -fno-inline")

include_directories(include)

add_executable(DemoTarget src/main.cpp)
target_include_directories(DemoTarget PRIVATE include)