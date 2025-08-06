<!-- Generated: 2025-07-08 00:00:00 UTC -->

# Files Catalog

RudeBase3D's codebase is organized for clarity and modularity. Core logic is in `src/` and `include/`, with tests, assets, and scripts in dedicated folders. Build and output files are in `build/`.

## Core Source Files
- `src/main.cpp`: Application entry point
- `src/core/`: Scene, entity, and manager logic
- `src/geometry/`: Geometry, mesh, and conversion utilities
- `src/rendering/`: Rendering systems
- `src/ui/`, `src/panels/`, `src/toolbars/`: UI components

## Platform Implementation
- Platform-specific code in `scripts/`, `CMakeLists.txt`
- Output in `build/bin/`, `build/lib/`

## Build System
- `CMakeLists.txt`: Main build config
- `scripts/build.bat`, `scripts/build.sh`: Build scripts

## Configuration
- `assets/`: Icons, materials, shaders
- `rudebase3d.code-workspace`: VSCode workspace config

## Reference
- File organization: See above
- Naming: Consistent with C++/Qt/GLM conventions
- Dependencies: See `third_party/`
