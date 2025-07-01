# RudeBase3D

A professional-grade 3D modeling application built with modern C++23 and Qt6, following industry-standard architecture patterns.

## 🎯 Project Vision

Ru## 📊 Current Status

- ✅ Basic 3D viewport and rendering
- ✅ Scene management system  
- ✅ Transform tools with gizmos
- ✅ Modular architecture implementation
- ✅ **Professional Theme System** - Multiple dark modes, auto-detection, real-time switching
- ✅ **Modern UI Components** - BaseComponent architecture, ThemeManager, ThemeSelector
- 🔄 Half-edge mesh operations
- 🔄 Advanced modeling tools
- ⏳ Plugin system
- ⏳ Advanced materials and lighting aims to be a complete 3D modeling solution comparable to industry standards like Blender or Maya, with a focus on:
- **Modern Architecture**: Clean, modular codebase following SOLID principles
- **Performance**: Optimized for real-time 3D workflows
- **Extensibility**: Plugin system for custom tools and workflows  
- **Professional UI**: Intuitive interface designed for productivity

## ✨ Features

### Core Functionality
- **3D Primitives**: Cube, Sphere, Cylinder, Plane generators with parametric controls
- **Advanced Geometry**: Half-edge mesh data structure for complex modeling operations
- **Hybrid Geometry System**: Seamless integration of mesh, NURBS, and voxel workflows
- **Professional Viewport**: Multi-view 3D rendering with modern OpenGL
- **Scene Management**: Hierarchical scene graph with transform inheritance

### Modeling Tools
- **Selection System**: Vertex, edge, face, and object selection modes
- **Transform Tools**: Move, rotate, scale with precise numerical input
- **Modeling Operations**: Extrude, bevel, loop cut, subdivide
- **Material System**: PBR materials with texture support

### User Interface
- **✅ Professional Theme System**: Multiple dark mode options (Modern Dark, High Contrast, Warm, Purple)
  - Real-time theme switching without restart
  - System dark mode auto-detection and following
  - User-friendly theme selector integrated in main toolbar
- **Modular UI**: Dockable panels with customizable layouts
- **Scene Hierarchy**: Tree view of all scene objects with visibility controls
- **Properties Panel**: Context-sensitive object and material properties
- **Tool Panels**: Organized tool palette with keyboard shortcuts
- **Responsive Design**: Theme-aware components with consistent styling

### Technical Features
- **Modern UI Architecture**: Comprehensive theme system with BaseComponent foundation
- **Command Pattern**: Full undo/redo system for all operations
- **Plugin Architecture**: Extensible system for custom tools and importers
- **File I/O**: Support for OBJ, STL, PLY formats with planned FBX/glTF support
- **Performance**: Optimized rendering pipeline with spatial data structures
- **Theme System**: Professional theming with multiple dark modes and auto-detection

## 🏗️ Building

### Prerequisites

- **Qt 6.5+** with OpenGL support
- **CMake 3.28+** (required for C++23 support)
- **Visual Studio 2022 17.8+** or GCC 13+ / Clang 17+
- **OpenGL 3.3+** compatible graphics card

### Quick Start

1. **Clone the repository**:
   ```bash
   git clone https://github.com/The-No-Hands-company/RudeBase3D.git
   cd RudeBase3D
   ```

2. **Build (Windows)**:
   ```batch
   scripts\build.bat
   ```

3. **Build (Unix)**:
   ```bash
   chmod +x scripts/build.sh
   ./scripts/build.sh
   ```

### Manual Build

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=23
cmake --build . --config Release --parallel
```

### Troubleshooting

#### Qt6 Not Found
If you get "Qt6 not found" errors, set the Qt path:

**Windows:**
```batch
cmake .. -DCMAKE_PREFIX_PATH="D:/Qt/6.6.1/msvc2022_64"
```

**Linux:**
```bash
cmake .. -DCMAKE_PREFIX_PATH="/opt/Qt/6.6.1/gcc_64"
```

**macOS:**
```bash
cmake .. -DCMAKE_PREFIX_PATH="/Users/$(whoami)/Qt/6.6.1/macos"
```

#### Common Issues
- **C++23 Support**: Ensure you have Visual Studio 2022 17.8+ or GCC 13+
- **OpenGL Issues**: Update your graphics drivers
- **Build Errors**: Try a clean build: `rm -rf build && mkdir build`

### Running

After building, the executable will be in `build/bin/RudeBase3D.exe`

## 🏛️ Architecture

RudeBase3D follows a modular, industry-standard architecture:

```
src/
├── core/           # Core engine systems (math, memory, utils)
├── geometry/       # Geometry processing (mesh, NURBS, voxels)
├── scene/          # Scene management and object hierarchy  
├── rendering/      # OpenGL rendering pipeline
├── tools/          # Modeling and transformation tools
├── ui/             # User interface (Qt-based)
├── input/          # Input handling and camera control
├── io/             # File import/export systems
├── commands/       # Command pattern for undo/redo
└── plugins/        # Plugin architecture
```

### Key Design Principles

- **Separation of Concerns**: Clear boundaries between UI, business logic, and rendering
- **Dependency Injection**: Loose coupling through interfaces and dependency inversion
- **Command Pattern**: All user actions are commands for robust undo/redo
- **Observer Pattern**: Event-driven updates between system components
- **Plugin Architecture**: Extensible system for custom tools and file formats

## 📖 Documentation

- **[🗺️ Development Roadmap](ROADMAP.md)** - Project phases and feature timeline
- **[⚙️ Git Workflow](docs/GIT_WORKFLOW.md)** - Contribution workflow and branching strategy
- **[🏛️ Project Governance](docs/GOVERNANCE.md)** - Leadership structure and decision making
- **[️ Architecture Overview](docs/MODERN_3D_ARCHITECTURE.md)** - Technical architecture details
- **[📹 Camera Controls](docs/CAMERA_CONTROLS.md)** - Viewport navigation guide
- **[🎨 Modeling Techniques](docs/modelingtechniques.md)** - User guides and tutorials
- **[📚 Full Documentation](docs/README.md)** - Complete documentation index

## 🤝 Contributing

We welcome contributions from developers, artists, designers, and users! 

- [📋 Contributing Guidelines](CONTRIBUTING.md) - How to contribute to the project
- [🔒 Security Policy](SECURITY.md) - Reporting security vulnerabilities  
- [📝 Changelog](CHANGELOG.md) - Project history and release notes

### Quick Start for Contributors
1. Read our [Contributing Guidelines](CONTRIBUTING.md)
2. Check the [Development Roadmap](docs/ROADMAP.md) for current priorities
3. Look for issues labeled `good first issue` 
4. Join our Discord community for discussions
5. Follow the [Branching Strategy](docs/BRANCHING_STRATEGY.md) for development

## 🤝 Community

Join our growing community of 3D artists, developers, and enthusiasts!

- **💬 Discord**: [The No Hands Company Community](https://discord.gg/YOUR_INVITE_HERE) *(Discord server setup guide available)*
- **🐛 Issues**: [GitHub Issues](https://github.com/The-No-Hands-company/RudeBase3D/issues)
- **💡 Discussions**: [GitHub Discussions](https://github.com/The-No-Hands-company/RudeBase3D/discussions)
- **📧 Email**: contact@no-hands-company.org

## 🚀 Development Status

### Current Phase: **Core Architecture**
- ✅ Basic 3D viewport and rendering
- ✅ Scene management system
- ✅ Transform tools with gizmos
- ✅ Modular architecture implementation
- 🔄 Half-edge mesh operations
- 🔄 Advanced modeling tools
- ⏳ Plugin system
- ⏳ Advanced materials and lighting

### Upcoming Features
- NURBS surface modeling
- Sculpting tools with dynamic topology
- Advanced selection modes
- Animation system
- Node-based material editor

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guidelines](CONTRIBUTING.md) for details.

### Development Setup

1. Follow the building instructions above
2. Enable all compiler warnings and treat them as errors
3. Run tests before submitting PRs: `cmake --build . --target test`
4. Use consistent code formatting (clang-format configuration provided)

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🏢 About The No Hands Company

RudeBase3D is developed by [The No Hands Company](https://github.com/The-No-Hands-company), focusing on innovative 3D software solutions.

RudeBase3D is built using modern C++23 features including:
- **Concepts** for type safety and better error messages
- **Ranges** for efficient data processing
- **std::expected** for error handling
- **constexpr** for compile-time optimizations
- **Spaceship operator** for automatic comparisons

## Usage

1. Launch RudeBase3D
2. Use the "Create" menu to add primitives to the scene
3. Select objects in the viewport or hierarchy panel
4. Use transform tools to move, rotate, and scale objects
5. Navigate the 3D view using mouse controls:
   - Left mouse: Orbit camera
   - Right mouse: Pan camera
   - Scroll wheel: Zoom

## License

MIT License - See LICENSE file for details
