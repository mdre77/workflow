cmake_minimum_required(VERSION 3.17)
project(Workflow)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/build/bin")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CompileFlags)

find_package(Boost REQUIRED)
find_package(GTest REQUIRED)
enable_testing()


add_subdirectory(utils)
add_subdirectory(type)