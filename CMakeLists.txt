
cmake_minimum_required(VERSION 4.0)
project(RudeBase3D VERSION 1.0.0 LANGUAGES CXX)

# Ensure GLM experimental extensions are enabled globally
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

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
find_package(Qt6 COMPONENTS Core Widgets Gui OpenGL OpenGLWidgets)


# ...existing code...


# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLM (header-only library)
find_package(glm QUIET)
if(NOT glm_FOUND)
    # Try to find GLM manually
    find_path(GLM_INCLUDE_DIRS glm/glm.hpp
        HINTS
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm
        /usr/include
        /usr/local/include
        DOC "GLM include directory"
    )
    if(GLM_INCLUDE_DIRS)
        message(STATUS "Found GLM: ${GLM_INCLUDE_DIRS}")
        set(GLM_FOUND TRUE)
    else()
        message(STATUS "GLM found in third_party/glm")
        set(GLM_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm")
        set(GLM_FOUND TRUE)
    endif()
endif()

# Find GLEW
find_path(GLEW_INCLUDE_DIRS GL/glew.h
    HINTS
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew/include
    DOC "GLEW include directory"
)

find_library(GLEW_LIBRARIES
    NAMES glew32 glew GLEW
    HINTS
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew/lib/Release/x64
    DOC "GLEW library"
)

if(GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)
    message(STATUS "Found GLEW: ${GLEW_LIBRARIES}")
    set(GLEW_FOUND TRUE)
else()
    message(STATUS "Using GLEW from third_party/glew")
    set(GLEW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew/include")
    # Try to build GLEW from source if library not found
    add_subdirectory(third_party/glew)
    set(GLEW_LIBRARIES glew)
    set(GLEW_FOUND TRUE)
endif()

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
    "include/*.hpp"
)

# Remove any backup or old files that might have been collected
list(FILTER SOURCES EXCLUDE REGEX ".*_backup\\.(cpp|h)$")
list(FILTER SOURCES EXCLUDE REGEX ".*_old\\.(cpp|h)$")
list(FILTER HEADERS EXCLUDE REGEX ".*_backup\\.(cpp|h)$")
list(FILTER HEADERS EXCLUDE REGEX ".*_old\\.(cpp|h)$")

# Add subdirectories for libraries
add_subdirectory(src/core)
add_subdirectory(src/ecs)
add_subdirectory(src/gizmo)
add_subdirectory(src/panels)
add_subdirectory(src/toolbars)
add_subdirectory(src/event)
add_subdirectory(src/input)


# Create executable
add_executable(RudeBase3D ${SOURCES} ${HEADERS})

# Ensure Qt6 include directories are available to all targets (after target is created)
if(Qt6_FOUND)
    # Use Qt6::Core, Qt6::Gui, etc. targets to propagate include directories and definitions
    # This is the modern CMake way and works with Qt6's CMake integration
    get_target_property(QT_CORE_INCLUDES Qt6::Core INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(QT_GUI_INCLUDES Qt6::Gui INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(QT_WIDGETS_INCLUDES Qt6::Widgets INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(QT_OPENGL_INCLUDES Qt6::OpenGL INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(QT_OPENGLWIDGETS_INCLUDES Qt6::OpenGLWidgets INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(RudeBase3D PRIVATE
        ${QT_CORE_INCLUDES}
        ${QT_GUI_INCLUDES}
        ${QT_WIDGETS_INCLUDES}
        ${QT_OPENGL_INCLUDES}
        ${QT_OPENGLWIDGETS_INCLUDES}
    )
else()
    message(FATAL_ERROR "Qt6 not found! Please set CMAKE_PREFIX_PATH or Qt6_DIR to your Qt installation.\n"
                        "Examples:\n"
                        "  Windows: -DCMAKE_PREFIX_PATH=\"D:/Qt/6.9.1/msvc2022_64\"\n"
                        "Or set Qt6_DIR directly:\n"
                        "  -DQt6_DIR=\"D:/Qt/6.9.1/msvc2022_64/lib/cmake/Qt6\"")
endif()
message(STATUS "Found Qt6: ${Qt6_VERSION} at ${Qt6_DIR}")

# Include directories for organized structure
target_include_directories(RudeBase3D PRIVATE 
    include
    include/core
    include/ecs
    include/gizmo
    include/toolbars
    include/panels
    include/event
    include/input
    src
    src/core
    src/ecs
    src/gizmo
    src/toolbars
    src/panels
    src/event
    src/input
    src/geometry/core
    src/geometry/halfedge  
    src/geometry/hybrid
    src/geometry/primitives
    src/scene
    src/rendering/core
    src/rendering/opengl
    src/rendering/effects
    src/ui/core
    src/ui/components
    src/ui/windows
    src/ui/panels
    src/ui/viewport
    src/ui/dialogs
    src/ui/menus
    src/ui/themes
    src/tools/base
    src/tools/selection
    src/tools/modeling
    src/input
    src/io/core
    src/io/formats
    src/io/project
    src/commands
    third_party/glm
    third_party/fmt/include
    third_party/imgui
    third_party/open3d/cpp
    third_party/eigen
    third_party/glew/include
)

# Add GLM include directory if found
if(GLM_FOUND AND GLM_INCLUDE_DIRS)
    target_include_directories(RudeBase3D PRIVATE ${GLM_INCLUDE_DIRS})
endif()

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
    Qt6::Gui
    Qt6::OpenGL 
    Qt6::OpenGLWidgets
    OpenGL::GL
    ${GLEW_LIBRARIES}
    core
    ecs
    gizmo
    panels
    toolbars
    event
    input
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
        _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING # Suppress MSVC deprecation warning for stdext::checked_array_iterator
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
