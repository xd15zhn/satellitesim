cmake_minimum_required(VERSION 3.12)
project(untitled VERSION 0.1.0)
set(CMAKE_BUILD_TYPE release)
set(CPP_LIBRARIES_PATH "E:/cpplibraries/")
list(APPEND CMAKE_PREFIX_PATH ${CPP_LIBRARIES_PATH})

set(PROJECT_SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/src/rcamera.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CPP_LIBRARIES_PATH}/include
    ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${CPP_LIBRARIES_PATH}/lib/libraylib.a
    opengl32 gdi32 winmm
)
