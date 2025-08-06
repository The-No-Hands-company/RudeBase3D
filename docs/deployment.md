<!-- Generated: 2025-07-08 00:00:00 UTC -->

# Deployment

RudeBase3D is distributed as a standalone binary with supporting assets. Packaging is platform-specific, with scripts and CMake targets for Windows and Unix-like systems.

## Package Types
- Windows: ZIP archive with `bin/`, `assets/`, and required DLLs
- Linux/macOS: Tarball with binaries and assets

## Platform Deployment
- Windows: Use `scripts/build.bat` and CMake install targets
- Linux/macOS: Use `scripts/build.sh` and CMake install
- Output: `build/bin/`, `build/assets/`

## Reference
- Deployment scripts: `scripts/build.bat`, `scripts/build.sh`
- Output locations: `build/bin/`, `build/assets/`
- Server configs: N/A (desktop app)
