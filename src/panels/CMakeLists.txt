add_library(panels
    outliner_panel.cpp
    properties_panel.cpp
    selection_panel.cpp
)

target_include_directories(panels PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(panels PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    core
)

set_property(TARGET panels PROPERTY AUTOMOC ON)
set_property(TARGET panels PROPERTY AUTOUIC ON)
