# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.1...3.25)

project(TinyNtp
  VERSION 0.1
  DESCRIPTION "A Tiny NTP client implementation."
  LANGUAGES C CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_EXTENSIONS OFF)

  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  find_package(GTest)
  if(GTest_FOUND AND BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
  endif()

endif()

add_library(timestamp_format timestamp_format.c timestamp_format.h)
target_include_directories(timestamp_format PUBLIC .)
