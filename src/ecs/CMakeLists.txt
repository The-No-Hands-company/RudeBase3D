# ECS (Entity Component System) module

# Collect ECS source files
file(GLOB_RECURSE ECS_SOURCES
    "*.cpp"
)

# Collect ECS header files from include directory
file(GLOB_RECURSE ECS_HEADERS
    "${CMAKE_SOURCE_DIR}/include/ecs/*.h"
    "${CMAKE_SOURCE_DIR}/include/ecs/*.hpp"
)

# Create ECS static library
add_library(ecs STATIC ${ECS_SOURCES} ${ECS_HEADERS})

# Set target properties
set_target_properties(ecs PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Include directories for ECS module
target_include_directories(ecs PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/ecs
    ${CMAKE_SOURCE_DIR}/third_party/glm/glm
)

# Link GLM if found
if(GLM_FOUND AND GLM_INCLUDE_DIRS)
    target_include_directories(ecs PUBLIC ${GLM_INCLUDE_DIRS})
endif()

# Export the library for the main target
set(ECS_LIBRARY ecs PARENT_SCOPE)

message(STATUS "ECS module configured with ${ECS_SOURCES}")
message(STATUS "ECS headers: ${ECS_HEADERS}")
