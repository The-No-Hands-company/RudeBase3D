cmake_minimum_required(VERSION 3.10)

# Set debug postfix for all Bullet3 libraries
set(CMAKE_DEBUG_POSTFIX "_d")

# Ensure all Bullet3 libraries output .lib/.dll/.pdb to build/lib/<config>
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set library output name and suffix
set(CMAKE_IMPORT_LIBRARY_PREFIX "")
set(CMAKE_IMPORT_LIBRARY_SUFFIX ".lib")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".dll")

# Add Bullet3 subdirectory
add_subdirectory(bullet3-3.25) 