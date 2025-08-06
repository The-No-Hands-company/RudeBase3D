cmake_minimum_required(VERSION 3.20)
project(nlohmann_json)
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/json-3.11.3/include) 