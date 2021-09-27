cmake_minimum_required(VERSION 3.18)
project(RavenEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

### Conan ###
set(CONAN_DISABLE_CHECK_COMPILER OFF)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)
link_libraries(${CONAN_LIBS})

### Third Party ###
include_directories(third-party/include)
file(GLOB_RECURSE THIRD_PARTY third-party/lib/libktx.so)
link_libraries(${THIRD_PARTY})

### Bindings ###
include_directories(bindings)
file(GLOB_RECURSE IMGUI_BINDINGS bindings/*.cpp)

### Source ###
include_directories(source)
add_subdirectory(source)
link_libraries(RavenEngine)

### Samples ###
add_subdirectory(samples)

### Tests ###
add_subdirectory(tests)