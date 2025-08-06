<!-- Generated: 2025-07-08 00:00:00 UTC -->

# Testing

RudeBase3D uses Catch2 for unit and integration testing. Tests are organized by subsystem and can be run via CTest or directly from the build output.

## Test Types
- Unit tests: `tests/unit/`
- Integration tests: `tests/integration/`
- Benchmarks: `tests/benchmark/`

## Running Tests
- CTest: `ctest` from `build/`
- Direct: Run test binaries in `build/tests/`

## Reference
- Test sources: `tests/unit/`, `tests/integration/`, `tests/benchmark/`
- Build integration: `CMakeLists.txt` (test targets)
- Output: `build/tests/`
