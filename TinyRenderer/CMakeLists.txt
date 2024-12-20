project(TinyRenderer)

set(SOURCE_FILES src/main.cpp src/tgaimage.cpp)
set(HEADER_FILES include/tgaimage.h)
set(INCLUDE_DIRS include ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})