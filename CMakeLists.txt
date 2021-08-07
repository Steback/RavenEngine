cmake_minimum_required(VERSION 3.18)
project(RavenEngine)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

### Source ###
include_directories(${PROJECT_SOURCE_DIR}/sources)
add_subdirectory(${PROJECT_SOURCE_DIR}/sources)

### ThirdParty ###
include_directories(${PROJECT_SOURCE_DIR}/thirdparty/include)

### Samples ###
add_subdirectory(${PROJECT_SOURCE_DIR}/samples)