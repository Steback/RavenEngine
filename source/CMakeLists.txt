file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_library(RavenEngine ${SOURCE_FILES} ${IMGUI_BINDINGS} external/libktx.so)