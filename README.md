# AHCCS Attendance Calculator

## Building:

### Prerequisites:
1. Must be on Windows
2. Must have [CMake](https://cmake.org/download/) installed
3. Must have [mingw-w64](http://www.mingw-w64.org/doku.php) installed

### Steps to build:
1. Navigate to the build directory ```cd build```
2. Run CMake to configure the project for MinGW ```cmake ../ -G "MinGW Makefiles```
3. Build the project with mingw-w64 ```mingw32-make```
4. MinGW will create a release directory in build. This will contain the executable for the calculator.
