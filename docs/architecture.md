<!-- Generated: 2025-07-08 00:00:00 UTC -->

# Architecture

RudeBase3D is organized into modular subsystems for geometry, rendering, file I/O, and UI. The core is the `rude::` namespace, which provides mesh, half-edge, scene, and entity APIs. The architecture supports extensibility and separation of concerns for geometry processing, scene management, and rendering.

## Component Map
- Geometry Core: `src/geometry/core/`, `include/core/mesh.hpp`, `include/core/half_edge_mesh.hpp`
- Scene Management: `src/core/scene.cpp`, `include/core/scene.hpp`, `src/core/scene_manager.cpp`
- Rendering: `src/rendering/core/RenderSystem.cpp`, `include/core/renderer.hpp`
- File I/O: `src/geometry/hybrid/GeometryConverter.cpp`, `src/geometry/primitives/MeshGenerator.cpp`
- UI: `src/ui/`, `src/panels/`, `src/toolbars/`

## Key Files
- `include/core/mesh.hpp`: Mesh API
- `include/core/half_edge_mesh.hpp`: Half-edge mesh API
- `include/core/scene.hpp`: Scene graph and entity management
- `src/core/scene.cpp`: Scene implementation
- `src/core/scene_manager.cpp`: Scene manager logic
- `src/geometry/primitives/MeshGenerator.cpp`: Mesh generation utilities

## Data Flow
- User actions (UI) → SceneManager (`src/core/scene_manager.cpp`) → Scene/Entity (`src/core/scene.cpp`) → Mesh/HalfEdgeMesh (`include/core/mesh.hpp`, `include/core/half_edge_mesh.hpp`) → Rendering (`src/rendering/core/RenderSystem.cpp`)
