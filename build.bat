@echo off
rem Create output/build directory if it doesn't exist
if not exist output\build mkdir output\build

rem Change to output/build directory
cd output\build

rem Run cmake to generate the build system in Release mode
cmake -DCMAKE_BUILD_TYPE=Release ../..


rem Build the project
cmake --build . --config Release

rem Run the resulting program
echo Running Engine3D program:
Release\Engine3D.exe

rem Return to the root directory
cd ../..