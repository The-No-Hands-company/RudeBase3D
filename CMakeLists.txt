cmake_minimum_required(VERSION 3.28)
project(RudeBase3D VERSION 1.0.0 LANGUAGES CXX)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ensure we're using the latest MSVC toolset for C++23 support
if(MSVC)
    # Use the latest MSVC toolset and enable C++23 features
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
    set(CMAKE_GENERATOR_TOOLSET "host=x64")
endif()

# Find required Qt components
message(STATUS "Looking for Qt6...")
find_package(Qt6 COMPONENTS Core Widgets OpenGL OpenGLWidgets)
if(NOT Qt6_FOUND)
    message(FATAL_ERROR "Qt6 not found! Please set CMAKE_PREFIX_PATH or Qt6_DIR to your Qt installation.\n"
                        "Examples:\n"
                        "  Windows: -DCMAKE_PREFIX_PATH=\"D:/Qt/6.6.1/msvc2022_64\"\n"
                        "  Linux:   -DCMAKE_PREFIX_PATH=\"/opt/Qt/6.6.1/gcc_64\"\n"
                        "  macOS:   -DCMAKE_PREFIX_PATH=\"/Users/$(whoami)/Qt/6.6.1/macos\"\n"
                        "Or set Qt6_DIR directly:\n"
                        "  -DQt6_DIR=\"/path/to/Qt/6.6.1/lib/cmake/Qt6\"")
endif()
message(STATUS "Found Qt6: ${Qt6_VERSION} at ${Qt6_DIR}")

# Find OpenGL
find_package(OpenGL REQUIRED)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Collect source files from organized directories
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Collect header files from organized directories  
file(GLOB_RECURSE HEADERS
    "src/*.h"
)

# Remove any backup or old files that might have been collected
list(FILTER SOURCES EXCLUDE REGEX ".*_backup\\.(cpp|h)$")
list(FILTER SOURCES EXCLUDE REGEX ".*_old\\.(cpp|h)$")
list(FILTER HEADERS EXCLUDE REGEX ".*_backup\\.(cpp|h)$")
list(FILTER HEADERS EXCLUDE REGEX ".*_old\\.(cpp|h)$")

# Create executable
add_executable(RudeBase3D ${SOURCES} ${HEADERS})

# Include directories for organized structure
target_include_directories(RudeBase3D PRIVATE 
    src
    src/core
    src/geometry/core
    src/geometry/halfedge  
    src/geometry/hybrid
    src/geometry/primitives
    src/scene
    src/rendering/core
    src/rendering/opengl
    src/rendering/effects
    src/ui/core
    src/ui/windows
    src/ui/panels
    src/ui/viewport
    src/tools/base
    src/tools/selection
    src/tools/modeling
    src/input
    src/io/core
    src/io/formats
    src/io/project
    src/commands
)

# Set target properties for modern C++
set_target_properties(RudeBase3D PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
)

# Enable modern C++ features
target_compile_features(RudeBase3D PRIVATE cxx_std_23)

# Link libraries
target_link_libraries(RudeBase3D 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::OpenGL 
    Qt6::OpenGLWidgets
    OpenGL::GL
)

# Set compiler-specific options for Visual Studio 2022 and C++23
if(MSVC)
    # Enable all reasonable warnings and latest C++ features
    target_compile_options(RudeBase3D PRIVATE 
        /W4                     # Warning level 4
        /permissive-           # Strict conformance mode
        /std:c++latest         # Use latest C++ standard (C++23)
        /Zc:__cplusplus        # Enable correct __cplusplus macro
        /Zc:preprocessor       # Enable conforming preprocessor
    )
    target_compile_definitions(RudeBase3D PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX               # Prevent min/max macro conflicts
        WIN32_LEAN_AND_MEAN    # Reduce Windows header bloat
    )
    
    # Enable modern C++23 features and optimizations
    set_target_properties(RudeBase3D PROPERTIES
        VS_GLOBAL_VcpkgEnabled false
        VS_GLOBAL_UseMultiToolTask true
        VS_GLOBAL_EnforceProcessCountAcrossBuilds true
    )
endif()

# Set output directory
set_target_properties(RudeBase3D PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin
)
