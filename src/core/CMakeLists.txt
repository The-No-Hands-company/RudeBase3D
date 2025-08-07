add_library(core
    core_system.cpp
    edit_preview_system.cpp
    global_shaders.cpp
    half_edge_mesh.cpp
    mesh.cpp
    mesh_iterator.cpp
    scene.cpp
    scene_manager.cpp
    selection_manager.cpp
    shader_utils.cpp
    entity.cpp
    transform.cpp
)

target_include_directories(core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>  # Add src directory for geometry/core includes
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../dependencies/glm/glm> # For glm usage in core
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/spdlog/include> # For spdlog
)

target_link_libraries(core PUBLIC
    Qt6::Core # For QDebug, QThread, etc.
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL # For QOpenGLFunctions_3_3_Core
    Qt6::OpenGLWidgets
)

set_property(TARGET core PROPERTY AUTOMOC ON)
set_property(TARGET core PROPERTY AUTOUIC ON)