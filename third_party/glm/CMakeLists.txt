cmake_minimum_required(VERSION 3.20)
project(glm)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/glm
)

# Install headers
install(DIRECTORY glm/
    DESTINATION include/glm
    FILES_MATCHING PATTERN "*.hpp"
)
