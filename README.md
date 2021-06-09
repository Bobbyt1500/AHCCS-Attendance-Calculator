# AHCCS Attendance Calculator

## Building:

### Prerequisites:
1. Must be on Windows
2. Must have [CMake](https://cmake.org/download/) installed
3. Must have [mingw-w64](http://www.mingw-w64.org/doku.php) installed

### Steps to build:
1. Clone the repository with the --recursive-submodules flag ```git clone --recurse-submodules https://github.com/Bobbyt1500/AHCCS-Attendance-Calculator```
1. Create and navigate to a build directory ```mkdir build``` then ```cd build```
2. Run CMake to configure the project for MinGW ```cmake ../ -G "MinGW Makefiles"```
3. Build the project with mingw-w64 ```mingw32-make```
4. MinGW will create a release directory in build. This will contain the executable for the calculator.

### Notes:
To toggle the raylib debugging console, comment or uncomment line 16 in [CMakeLists.txt](CMakeLists.txt)
