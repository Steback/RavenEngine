cmake_minimum_required(VERSION 3.18)
project(RavenEngine)

set(CMAKE_CXX_STANDARD 17)

### Conan ###
set(CONAN_DISABLE_CHECK_COMPILER OFF)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

### Compile Shaders ###
set(COMPILE_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/tools/compileShaders.py ${CMAKE_CURRENT_SOURCE_DIR})
execute_process(COMMAND python ${COMPILE_SCRIPT})

### Bindings ###
file(GLOB_RECURSE IMGUI_BINDINGS bindings/*.cpp)

### Source ###
include_directories(source)
add_subdirectory(source)

### Samples ###
add_subdirectory(samples)

### Tests ###
add_subdirectory(tests)