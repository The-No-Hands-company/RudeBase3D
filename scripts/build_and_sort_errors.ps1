# Automated build, error collection, and sorting script

# Step 1: Configure CMake if needed
if (-not (Test-Path 'build/CMakeCache.txt')) {
    if (-not (Test-Path 'build')) { New-Item -ItemType Directory -Path 'build' }
    Push-Location build
    if ($env:TOOLCHAIN -eq "mingw64") {
        cmake .. -G "MinGW Makefiles"
    } else {
        cmake .. -G "Visual Studio 17 2022" -A x64
    }
    Pop-Location
}

# Step 2: Build and collect errors
cmake --build build --config Debug 2>&1 | Tee-Object -FilePath build_errors.txt

# Step 3: Sort errors (if sort_errors.ps1 exists)
if (Test-Path 'scripts/sort_errors.ps1') {
    pwsh -NoProfile -ExecutionPolicy Bypass -File scripts/sort_errors.ps1
    if (Test-Path 'build/sorted_build_errors.txt') {
        Copy-Item 'build/sorted_build_errors.txt' 'sorted_build_errors.txt' -Force
    } elseif (Test-Path 'sorted_build_errors.txt') {
        # Already in root
    }
}
Write-Host 'Build and error sorting complete.'
