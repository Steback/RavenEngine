set(SAMPLE_NAME Cube)

file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable(${SAMPLE_NAME} ${SOURCE_FILES})
target_link_libraries(${SAMPLE_NAME} RavenEngine ${CONAN_LIBS} ${THIRD_PARTY})
