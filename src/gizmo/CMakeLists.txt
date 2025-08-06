add_library(gizmo
    gizmo.cpp
    gizmo_manager.cpp
    rotate_gizmo.cpp
    scale_gizmo.cpp
    translate_gizmo.cpp
)

target_include_directories(gizmo PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>  # Add src directory for scene/Camera.h includes
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(gizmo PUBLIC
    core
    event
)

set_property(TARGET gizmo PROPERTY AUTOMOC ON)
set_property(TARGET gizmo PROPERTY AUTOUIC ON)