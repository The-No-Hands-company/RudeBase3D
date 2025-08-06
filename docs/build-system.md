<!-- Generated: 2025-07-08 00:00:00 UTC -->

# Build System

RudeBase3D uses CMake for cross-platform builds, with scripts for Windows and Unix-like systems. The build produces binaries in `build/bin/` and libraries in `build/lib/`.

## Build Workflows
- Configure: `cmake .. -G "Visual Studio 17 2022" -A x64` (Windows), `cmake ..` (Linux/macOS)
- Build: `cmake --build . --config Debug` (Windows), `cmake --build .` (Linux/macOS)
- Scripts: `scripts/build.bat` (Windows), `scripts/build.sh` (Unix)

## Platform Setup
- Windows: Visual Studio 2022, PowerShell, batch scripts
- Linux/macOS: GCC/Clang, shell scripts
- See `CMakeLists.txt` and `scripts/` for details

## Reference
- Main config: `CMakeLists.txt`
- Build scripts: `scripts/build.bat`, `scripts/build.sh`
- Output: `build/bin/`, `build/lib/`
- Troubleshooting: See `build/build_errors.txt`
