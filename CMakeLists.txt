cmake_minimum_required(VERSION 3.1)

project(CCGHomework)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "-fpermissive -std=c++17")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory("harmonic_map")
