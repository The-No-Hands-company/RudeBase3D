add_library(event
    event_dispatcher.cpp
    mouse_handler.cpp
    keyboard_handler.cpp
    gizmo_handler.cpp
)

target_include_directories(event PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>  # Add src directory for scene/Camera.h includes
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(event PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)

set_property(TARGET event PROPERTY AUTOMOC ON)
