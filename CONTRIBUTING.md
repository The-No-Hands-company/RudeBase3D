# Contributing to RudeBase3D

Thank you for your interest in contributing to RudeBase3D! This document provides guidelines for contributing to the project.

## 🎯 Project Goals

RudeBase3D aims to be a professional-grade 3D modeling application with:
- Clean, maintainable architecture
- High performance for real-time workflows  
- Extensible plugin system
- Industry-standard user experience

## 🚀 Getting Started

### Development Environment

1. **Prerequisites**:
   - Visual Studio 2022 (Windows) or equivalent C++23 compiler
   - Qt 6.9+ with OpenGL support
   - CMake 4.0+
   - Git

2. **Setup**:
   ```bash
   git clone https://github.com/The-No-Hands-company/RudeBase3D.git
   cd RudeBase3D
   # Follow build instructions in README.md
   ```

3. **Verify Setup**:
   ```bash
   # Build and run tests
   mkdir build && cd build
   cmake .. && cmake --build . --config Debug
   cmake --build . --target test
   ```

## 📝 Code Standards

### C++ Guidelines

- **C++23**: Use modern C++ features appropriately
- **Const Correctness**: Mark functions and variables const when possible
- **RAII**: Use smart pointers and automatic resource management
- **Naming**: 
  - Classes: `PascalCase` (e.g., `MeshGenerator`)
  - Functions/Variables: `camelCase` (e.g., `generateMesh()`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_VERTICES`)
  - Private members: trailing underscore (e.g., `meshData_`)

### Architecture Principles

- **Single Responsibility**: Each class should have one clear purpose
- **Dependency Injection**: Use interfaces to decouple components
- **Command Pattern**: All user actions should be undoable commands
- **Namespace Organization**: Use proper namespaces (`RudeBase3D::Geometry`, etc.)

### File Organization

```
src/
├── module_name/
│   ├── ClassName.h      # Interface in header
│   ├── ClassName.cpp    # Implementation
│   └── tests/           # Unit tests for module
```

## 🔄 Development Workflow

### 1. Issues and Planning

- Check existing [issues](https://github.com/The-No-Hands-company/RudeBase3D/issues)
- Create an issue for new features or bugs
- Discuss approach before implementing large features

### 2. Branching Strategy

```bash
# Create feature branch
git checkout -b feature/your-feature-name

# Or bug fix branch  
git checkout -b fix/bug-description
```

### 3. Making Changes

1. **Write Tests First**: Add unit tests for new functionality
2. **Implement**: Write the minimal code to make tests pass
3. **Refactor**: Improve code quality while keeping tests green
4. **Document**: Update documentation and comments

### 4. Commit Guidelines

```bash
# Format: type(scope): description
# Examples:
git commit -m "feat(geometry): add half-edge mesh subdivision"
git commit -m "fix(ui): resolve viewport rendering artifacts"
git commit -m "docs: update build instructions for Qt 6.6"
```

**Commit Types**:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `refactor`: Code refactoring
- `test`: Test additions/changes
- `chore`: Build/tooling changes

### 5. Pull Request Process

1. **Update Documentation**: Ensure README and docs are current
2. **Run Tests**: All tests must pass
3. **Code Review**: Submit PR for review
4. **Address Feedback**: Make requested changes
5. **Merge**: Squash commits if requested

## 🧪 Testing

### Unit Tests

- Write tests for all public interfaces
- Use descriptive test names: `test_MeshGenerator_CreatesCubeWithCorrectVertexCount`
- Test edge cases and error conditions
- Mock dependencies to isolate units under test

### Integration Tests

- Test component interactions
- Focus on critical user workflows
- Use realistic test data

### Running Tests

```bash
# Run all tests
cmake --build . --target test

# Run specific test suite
./tests/geometry_tests

# Run with verbose output
ctest --verbose
```

## 📚 Documentation

### Code Documentation

- **Headers**: Document all public interfaces with Doxygen comments
- **Implementation**: Explain complex algorithms and design decisions
- **Examples**: Provide usage examples for new APIs

### User Documentation

- Update user-facing documentation for new features
- Include screenshots and step-by-step guides
- Keep documentation in sync with code changes

## 🎨 UI/UX Guidelines

### Design Principles

- **Consistency**: Follow established UI patterns
- **Accessibility**: Support keyboard navigation and screen readers
- **Performance**: UI should remain responsive during heavy operations
- **Professional**: Interface should feel polished and production-ready

### Qt Specific

- Use Qt's model/view architecture for data display
- Implement proper signal/slot connections
- Follow Qt naming conventions for UI elements
- Support theming and high-DPI displays

## 🚀 Performance Guidelines

### Rendering

- Batch OpenGL calls where possible
- Use vertex buffer objects for geometry
- Implement frustum culling for large scenes
- Profile with graphics debuggers (RenderDoc, etc.)

### Memory Management

- Use smart pointers appropriately
- Implement object pooling for frequently created/destroyed objects
- Profile memory usage with Valgrind or similar tools
- Be conscious of cache-friendly data structures

### Algorithms

- Use spatial data structures (octrees, BVH) for scene queries
- Implement multi-threading for CPU-intensive operations
- Consider SIMD optimizations for math-heavy code

## 🐛 Debugging

### Common Issues

- **Qt/OpenGL Context**: Ensure proper OpenGL context management
- **Memory Leaks**: Use smart pointers and RAII patterns
- **Performance**: Profile before optimizing, measure after
- **Cross-Platform**: Test on multiple operating systems

### Tools

- **Debuggers**: Visual Studio debugger, GDB, LLDB
- **Memory**: AddressSanitizer, Valgrind
- **Graphics**: RenderDoc, NVIDIA Nsight
- **Profiling**: perf, Intel VTune

## 🌟 Feature Requests

### Before Proposing

1. Check if feature aligns with project goals
2. Search existing issues and discussions
3. Consider implementation complexity vs. benefit
4. Think about user experience impact

### Proposal Format

```markdown
## Feature: [Brief Description]

### Problem
What problem does this solve for users?

### Proposed Solution
How should this feature work?

### Implementation Notes
Technical considerations and approach

### Alternatives Considered
What other solutions were evaluated?
```

## 📞 Getting Help

- **Discussions**: Use GitHub Discussions for questions
- **Issues**: Report bugs with minimal reproduction steps
- **Code Review**: Request reviews from maintainers
- **Documentation**: Check existing docs and code comments

## 🏆 Recognition

Contributors will be:
- Listed in project credits
- Mentioned in release notes for significant contributions
- Invited to join the core development team for exceptional contributions

Thank you for contributing to RudeBase3D! 🎉
