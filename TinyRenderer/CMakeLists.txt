project(TinyRenderer)

set(SOURCE_FILES src/main.cpp src/tgaimage.cpp src/model.cpp)
set(HEADER_FILES include/tgaimage.h include/model.h include/geometry.h)
set(INCLUDE_DIRS include ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

set(ASSETS assets/african_head.obj)

include(../cmake/CopyAssets.cmake)
CopyAssetsPostBuild(${PROJECT_NAME} ${ASSETS})