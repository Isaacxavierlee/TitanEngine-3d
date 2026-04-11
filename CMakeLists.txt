cmake_minimum_required(VERSION 3.20)
project(TitanEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")
FetchContent_MakeAvailable(glfw)

# Engine library
add_library(TitanEngine STATIC
    Engine/src/Core/Engine.cpp
)

target_include_directories(TitanEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include
)

target_link_libraries(TitanEngine PUBLIC glfw)

# Editor executable
add_executable(TitanEditor
    Editor/src/Main.cpp
)

target_link_libraries(TitanEditor PRIVATE TitanEngine)

message(STATUS "TitanEngine configured!")