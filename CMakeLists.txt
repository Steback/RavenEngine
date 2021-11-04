cmake_minimum_required(VERSION 3.18)
project(RavenEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

### Conan ###
set(CONAN_DISABLE_CHECK_COMPILER OFF)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

### Bindings ###
include_directories(bindings)
file(GLOB_RECURSE IMGUI_BINDINGS bindings/*.cpp)

### Config ###
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

### Engine ###
include_directories(source)
add_subdirectory(source/engine)

### Editor ###
add_subdirectory(source/editor)