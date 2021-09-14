set(SAMPLE_NAME DamagedHelmet)

file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable(${SAMPLE_NAME} ${SOURCE_FILES})
target_link_libraries(${SAMPLE_NAME} RavenEngine ${CONAN_LIBS} ${PROJECT_SOURCE_DIR}/source/external/libktx.so)
