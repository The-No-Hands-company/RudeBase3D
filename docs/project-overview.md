<!-- Generated: 2025-07-08 00:00:00 UTC -->

# Project Overview

RudeBase3D is a modern, extensible 3D geometry and modeling application built with C++, Qt, and GLM. It provides a robust mesh and half-edge API (the `rude::` namespace), supporting advanced geometry operations, file format conversion, and real-time rendering. The project is designed for research, prototyping, and extensible 3D workflows.

## Key Files
- Main entry: `src/main.cpp`
- Core logic: `src/core/`, `src/geometry/`, `src/rendering/`
- Build config: `CMakeLists.txt`, `scripts/build.bat`, `scripts/build.sh`
- App config: `rudebase3d.code-workspace`

## Technology Stack
- C++17, Qt, GLM, OpenGL, ImGui, CMake
- Third-party: Assimp, Bullet3, Catch2, Eigen, FastNoise2, Fmt, Glad, GLFW, GLM, ImGui, JSON, LibIGL, Open3D, OpenVDB, PhysX, Pybind11, Spdlog, stb

## Platform Support
- Windows (Visual Studio 2022, PowerShell, batch)
- Linux/macOS (CMake, shell scripts)
- See `scripts/` and `CMakeLists.txt` for platform-specific setup

