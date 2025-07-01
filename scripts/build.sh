#!/bin/bash
# Cross-platform build script for RudeBase3D

set -e

echo "🏗️  Building RudeBase3D..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure CMake
echo "⚙️  Configuring CMake..."
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    # Windows
    cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=23
else
    # Unix-like systems
    cmake .. -DCMAKE_CXX_STANDARD=23 -DCMAKE_BUILD_TYPE=Release
fi

# Build
echo "🔨 Building project..."
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    cmake --build . --config Release --parallel
else
    make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
fi

echo "✅ Build complete! Executable is in build/bin/"
echo "🚀 Run with: ./build/bin/RudeBase3D"
