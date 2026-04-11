cmake_minimum_required(VERSION 3.20)
project(TitanEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")
FetchContent_MakeAvailable(glfw)

# Vulkan Headers
FetchContent_Declare(
    vulkan_headers
    GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
    GIT_TAG v1.3.290
)
FetchContent_MakeAvailable(vulkan_headers)

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.6-docking
)
FetchContent_MakeAvailable(imgui)

# Engine library
add_library(TitanEngine STATIC
    Engine/src/Core/Engine.cpp
    Engine/src/Core/ImGuiLayer.cpp
    Engine/src/Renderer/VulkanRenderer.cpp
)

# ImGui sources
target_sources(TitanEngine PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)

target_include_directories(TitanEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${vulkan_headers_SOURCE_DIR}/include
)

target_link_libraries(TitanEngine PUBLIC glfw)

if(WIN32)
    target_link_libraries(TitanEngine PRIVATE vulkan-1.lib)
endif()

# Editor
add_executable(TitanEditor
    Editor/src/Main.cpp
)

target_link_libraries(TitanEditor PRIVATE TitanEngine)

message(STATUS "TitanEngine with Vulkan configured!")