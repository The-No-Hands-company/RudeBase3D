cmake_minimum_required(VERSION 3.20)
project(libigl)
add_library(libigl INTERFACE)
target_include_directories(libigl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/libigl-2.5.0/include) 