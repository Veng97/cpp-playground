cmake_minimum_required(VERSION 3.10)
project(LivePlotter)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -gdwarf-4 -fno-omit-frame-pointer -fno-inline")

include_directories(include)

# Build library
add_library(plotter src/plotter/publisher.cpp src/plotter/jsonize.cpp)

# Add examples if BUILD_EXAMPLES is enabled
option(BUILD_EXAMPLES "Build example programs" ON)
if(BUILD_EXAMPLES)
  add_executable(ExampleTarget examples/example.cpp)
  target_link_libraries(ExampleTarget plotter)
endif()

# Add tests if BUILD_TESTS is enabled
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
  enable_testing()
  add_executable(JsonizeTests tests/jsonize_tests.cpp)
  target_link_libraries(JsonizeTests PRIVATE plotter gtest gtest_main)
  add_test(NAME JsonizeTests COMMAND JsonizeTests)
endif()
