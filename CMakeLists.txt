cmake_minimum_required(VERSION 3.7.2)
project(CPP)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(DEFINED ENV{BUILD_VERSION})
  set(BUILD_VERSION ENV{BUILD_VERSION})
else(DEFINED ENV{BUILD_VERSION})
  set(BUILD_VERSION 0.0.0)
endif(DEFINED ENV{BUILD_VERSION})
message("Using build version ${BUILD_VERSION}")

set(CPP_SRC_ROOT_DIR ${CPP_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "-std=c++11 -fstrict-aliasing -Wall -Werror")

enable_testing()

include_directories(
  ${CPP_SRC_ROOT_DIR}
  ${CPP_SRC_ROOT_DIR}/lib
)

add_subdirectory(src)

