file(GLOB_RECURSE SOURCE_FILES *.cpp)

set(EXEC_NAME Editor)
add_executable(${EXEC_NAME} ${SOURCE_FILES})
target_link_libraries(${EXEC_NAME} RavenEngine ${CONAN_LIBS})
