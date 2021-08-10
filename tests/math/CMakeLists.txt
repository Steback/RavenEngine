set(TEST_NAME MathTests)

file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable(${TEST_NAME} ${SOURCE_FILES})
target_link_libraries(${TEST_NAME} RavenEngine ${CONAN_LIBS})
