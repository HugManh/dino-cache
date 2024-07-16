cmake_minimum_required(VERSION 3.29.6)
project(dino_cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)
# file(GLOB SOURCES "src/*.cc")

add_executable(test_cache test.cc)
