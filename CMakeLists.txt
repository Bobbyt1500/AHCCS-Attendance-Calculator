cmake_minimum_required(VERSION 3.16)

# Use c++20
set(CMAKE_CXX_STANDARD 20)

project(Attendance_Calculator)

# Dont build raylib examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Set directory for executible to be placed
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release)

# Add flags to remove the raylib debugging console
# Comment out the below line to include the debugging console
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wl,--subsystem,windows")

# Copy program files to the release directory
file(COPY ${CMAKE_SOURCE_DIR}/programfiles/Resources DESTINATION ${CMAKE_BINARY_DIR}/release)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/release/Courses)
file(COPY ${CMAKE_SOURCE_DIR}/programfiles/userdata.json DESTINATION ${CMAKE_BINARY_DIR}/release)

include_directories("include")

# raygui
include_directories("libs/raygui/src")

# cpp-httplib
include_directories("libs/cpp-httplib")

# json
include_directories("libs/json/single_include/nlohmann")

# date
include_directories("libs/date/include/date")

# simpleini
include_directories("libs/simpleini")

add_subdirectory(libs/raylib)
add_subdirectory(src)

