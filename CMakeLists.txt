cmake_minimum_required(VERSION 3.18)
project(RavenEngine)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Bin)

### Source ###
include_directories(${PROJECT_SOURCE_DIR}/Source)
add_subdirectory(${PROJECT_SOURCE_DIR}/Source)

### Samples ###
add_subdirectory(${PROJECT_SOURCE_DIR}/Samples)