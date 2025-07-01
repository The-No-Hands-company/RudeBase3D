@echo off
echo Building RudeBase3D with Visual Studio 2022 and C++23...

REM Create build directory
if not exist build mkdir build
cd build

REM Configure with CMake for Visual Studio 2022 with C++23
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_CXX_STANDARD=23 -DCMAKE_CXX_STANDARD_REQUIRED=ON -T host=x64
if %errorlevel% neq 0 (
    echo CMake configuration failed!
    echo Make sure you have CMake 3.28+ and Visual Studio 2022 installed.
    pause
    exit /b 1
)

REM Build the project
cmake --build . --config Release --parallel
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo Build completed successfully!
echo Executable location: build\bin\Release\RudeBase3D.exe
pause
