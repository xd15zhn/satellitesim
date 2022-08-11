cmake_minimum_required(VERSION 3.12)
project(satellitesim VERSION 0.1.0)
set(CMAKE_BUILD_TYPE release)
set(CPP_LIBRARIES_PATH "E:/cpplibraries/")
list(APPEND CMAKE_PREFIX_PATH ${CPP_LIBRARIES_PATH})

set(PROJECT_SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/src/orbitalsim.cpp
    ${PROJECT_SOURCE_DIR}/src/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/skybox.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

find_package(simucpp REQUIRED)
find_package(raylib REQUIRED)
message(STATUS "simucpp_DIR: ${simucpp_DIR}")
message(STATUS "simucpp_VERSION: ${simucpp_VERSION}")
message(STATUS "raylib_DIR: ${raylib_DIR}")
message(STATUS "raylib_VERSION: ${raylib_VERSION}")
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CPP_LIBRARIES_PATH}/include
    ${PROJECT_SOURCE_DIR}/src
    ${simucpp_INCLUDE_DIRS}
    ${raylib_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${raylib_LIBS}
    ${simucpp_LIBS}
)
