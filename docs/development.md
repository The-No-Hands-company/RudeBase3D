<!-- Generated: 2025-07-08 00:00:00 UTC -->

# Development

RudeBase3D follows modern C++17/20 practices, with a focus on modularity, type safety, and extensibility. Code style is consistent with Qt and GLM conventions, and all geometry code uses the `rude::` namespace and API. See [docs/architecture.md](architecture.md) for system structure.

## Code Style
- Indent: 4 spaces, no tabs
- Namespace: All core types in `rude::`
- Includes: Use angle brackets for system/third-party, quotes for project headers
- Example (from `include/core/mesh.hpp`):
  ```cpp
  namespace rude {
  class Mesh { ... };
  }
  ```

## Common Patterns
- Use `std::shared_ptr<rude::Mesh>` for mesh ownership
- Prefer `glm::vec3` for positions/normals, `glm::vec2` for texcoords
- All mesh operations via `rude::Mesh` and `rude::HalfEdgeMesh`
- Scene/entity management via `rude::SceneManager` and `rude::Scene`

## Workflows
- Add geometry: Implement in `src/geometry/primitives/`, update `include/core/mesh.hpp`
- Add file format: Extend `src/geometry/hybrid/GeometryConverter.cpp`
- UI panels: Add to `src/panels/`, register in main window

## Reference
- File organization: `src/`, `include/`, `tests/`, `assets/`
- Naming: PascalCase for types, camelCase for functions, UPPER_SNAKE for constants
- Common issues: See `build/build_errors.txt` for build errors
