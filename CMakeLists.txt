cmake_minimum_required(VERSION 3.18)
project(RavenEngine)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH bin)

### ThirdParty ###
include_directories(thirdparty/include)
file(GLOB_RECURSE THIRD_PARTY_LIBS thirdparty/lib/*.a)

### Source ###
include_directories(sources)
add_subdirectory(sources)

### Samples ###
add_subdirectory(samples)