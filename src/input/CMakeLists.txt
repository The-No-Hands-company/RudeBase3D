add_library(input
    EventDrivenCameraController.cpp
    BlenderCameraController.cpp
    CameraController.cpp
    ICameraController.cpp
    InputController.cpp
    MayaCameraController.cpp
)

target_include_directories(input PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(input PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    event
    core
)

set_property(TARGET input PROPERTY AUTOMOC ON)
