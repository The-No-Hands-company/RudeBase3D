cmake_minimum_required(VERSION 3.20)
project(eigen)
add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/eigen-3.4.0) 