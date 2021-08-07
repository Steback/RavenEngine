cmake_minimum_required(VERSION 3.18)
project(RavenEngine)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH bin)

### ThirdParty ###
set(CONAN_DISABLE_CHECK_COMPILER OFF)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

### Source ###
include_directories(sources)
add_subdirectory(sources)

### Samples ###
add_subdirectory(samples)