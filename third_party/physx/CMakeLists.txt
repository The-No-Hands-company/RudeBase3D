cmake_minimum_required(VERSION 3.10)

# Set PhysX paths
set(PHYSX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/PhysX")
set(PHYSX_SOURCE_DIR "${PHYSX_ROOT}/physx")
set(PHYSX_BUILD_DIR "${PHYSX_ROOT}/physx/compiler/vc17win64")
set(PHYSX_BIN_DIR "${PHYSX_SOURCE_DIR}/bin/win.x86_64.vc142.mt")

# Set PhysX library paths
set(PHYSX_LIBRARY_DIR "${PHYSX_BIN_DIR}/debug")
set(PHYSX_INCLUDE_DIR "${PHYSX_SOURCE_DIR}/include")

# Add PhysX libraries
add_library(PhysX SHARED IMPORTED)
set_target_properties(PhysX PROPERTIES
    IMPORTED_LOCATION "${PHYSX_LIBRARY_DIR}/PhysX_64.dll"
    IMPORTED_IMPLIB "${PHYSX_LIBRARY_DIR}/PhysX_64.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${PHYSX_INCLUDE_DIR}"
)

add_library(PhysXCommon SHARED IMPORTED)
set_target_properties(PhysXCommon PROPERTIES
    IMPORTED_LOCATION "${PHYSX_LIBRARY_DIR}/PhysXCommon_64.dll"
    IMPORTED_IMPLIB "${PHYSX_LIBRARY_DIR}/PhysXCommon_64.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${PHYSX_INCLUDE_DIR}"
)

add_library(PhysXFoundation SHARED IMPORTED)
set_target_properties(PhysXFoundation PROPERTIES
    IMPORTED_LOCATION "${PHYSX_LIBRARY_DIR}/PhysXFoundation_64.dll"
    IMPORTED_IMPLIB "${PHYSX_LIBRARY_DIR}/PhysXFoundation_64.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${PHYSX_INCLUDE_DIR}"
)

add_library(PhysXCooking SHARED IMPORTED)
set_target_properties(PhysXCooking PROPERTIES
    IMPORTED_LOCATION "${PHYSX_LIBRARY_DIR}/PhysXCooking_64.dll"
    IMPORTED_IMPLIB "${PHYSX_LIBRARY_DIR}/PhysXCooking_64.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${PHYSX_INCLUDE_DIR}"
)

# Add static libraries
add_library(PhysXExtensions STATIC IMPORTED)
set_target_properties(PhysXExtensions PROPERTIES
    IMPORTED_LOCATION "${PHYSX_LIBRARY_DIR}/PhysXExtensions_static_64.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${PHYSX_INCLUDE_DIR}"
)

add_library(PhysXCharacterKinematic STATIC IMPORTED)
set_target_properties(PhysXCharacterKinematic PROPERTIES
    IMPORTED_LOCATION "${PHYSX_LIBRARY_DIR}/PhysXCharacterKinematic_static_64.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${PHYSX_INCLUDE_DIR}"
)

add_library(PhysXVehicle STATIC IMPORTED)
set_target_properties(PhysXVehicle PROPERTIES
    IMPORTED_LOCATION "${PHYSX_LIBRARY_DIR}/PhysXVehicle_static_64.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${PHYSX_INCLUDE_DIR}"
)

# Create an ALIAS for PhysX
add_library(PhysX::PhysX ALIAS PhysX)

# Create a meta-target for all PhysX components (no colons)
add_library(PhysXAll INTERFACE)
target_link_libraries(PhysXAll INTERFACE
    PhysX
    PhysXCommon
    PhysXFoundation
    PhysXCooking
    PhysXExtensions
    PhysXCharacterKinematic
    PhysXVehicle
) 