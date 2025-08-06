add_library(toolbars
    base_toolbar.cpp
    main_toolbar.cpp
    primitives_toolbar.cpp
    selection_toolbar.cpp
    toolbar_manager.cpp
    transform_toolbar.cpp
)

target_include_directories(toolbars PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(toolbars PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    core
)

set_property(TARGET toolbars PROPERTY AUTOMOC ON)
set_property(TARGET toolbars PROPERTY AUTOUIC ON)
