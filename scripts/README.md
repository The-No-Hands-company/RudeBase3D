# Development Environment Setup

This directory contains scripts to help set up and manage the RudeBase3D development environment.

## Scripts

### `build.bat` (Windows)
Primary build script for Windows development with Visual Studio.

### `build.sh` (Unix/Linux/macOS)  
Cross-platform build script for Unix-like systems.

## Usage

### Windows
```batch
scripts\build.bat
```

### Unix/Linux/macOS
```bash
chmod +x scripts/build.sh
./scripts/build.sh
```

## Requirements

- **CMake 3.28+**
- **Qt 6.5+** with OpenGL support
- **C++23** compatible compiler:
  - Windows: Visual Studio 2022 17.8+
  - Linux: GCC 13+ or Clang 17+
  - macOS: Xcode 15+ or Clang 17+

## Environment Variables

The scripts will automatically detect your environment and configure the build appropriately. For custom Qt installations, you may need to set:

```bash
export Qt6_DIR=/path/to/qt6/lib/cmake/Qt6
```

## Troubleshooting

### Qt Not Found
Ensure Qt6 is installed and either in your PATH or Qt6_DIR is set.

### C++23 Support Issues
Verify your compiler supports C++23:
- Visual Studio: Version 17.8 or later
- GCC: Version 13 or later  
- Clang: Version 17 or later

### Build Errors
Try a clean build:
```bash
rm -rf build
./scripts/build.sh
```
