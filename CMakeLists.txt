cmake_minimum_required(VERSION 3.20)
project(TitanEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Suppress deprecation warnings from fetched dependencies
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

# GLEW_STATIC must be defined globally BEFORE glew is fetched
add_compile_definitions(GLEW_STATIC)

include(FetchContent)

# ── GLFW ──────────────────────────────────────────────────────────────────────
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# ── GLEW (static) ─────────────────────────────────────────────────────────────
FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG        glew-cmake-2.2.0
)
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(glew-cmake_BUILD_STATIC ON  CACHE BOOL "" FORCE)
set(ONLY_LIBS               ON  CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glew)

# ── GLM ───────────────────────────────────────────────────────────────────────
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# ── ImGui ─────────────────────────────────────────────────────────────────────
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.90.6-docking
)
FetchContent_MakeAvailable(imgui)

# ── OpenGL ────────────────────────────────────────────────────────────────────
find_package(OpenGL REQUIRED)

# ── Verify every engine .cpp exists ───────────────────────────────────────────
set(ENGINE_SOURCES
    Engine/src/Core/Engine.cpp
    Engine/src/Core/LayerStack.cpp
    Engine/src/Core/ImGuiLayer.cpp
    Engine/src/Core/Input.cpp
    Engine/src/Core/Renderer/GraphicsContext.cpp
    Engine/src/Core/Renderer/Buffer.cpp
    Engine/src/Core/Renderer/VertexArray.cpp
    Engine/src/Core/Renderer/Shader.cpp
    Engine/src/Core/Renderer/Camera.cpp
    Engine/src/Core/Renderer/Renderer.cpp
)

foreach(src ${ENGINE_SOURCES})
    set(full "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    if(NOT EXISTS "${full}")
        message(FATAL_ERROR "\n  MISSING: ${src}\n  Expected at: ${full}\n  Create this file then re-run cmake.\n")
    endif()
endforeach()

# ── TitanEngine static library ────────────────────────────────────────────────
add_library(TitanEngine STATIC
    ${ENGINE_SOURCES}
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(TitanEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine/include
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${glfw_SOURCE_DIR}/include
    ${glew_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
)

# ── Resolve GLEW static target ────────────────────────────────────────────────
if(TARGET libglew_static)
    set(GLEW_LIB libglew_static)
elseif(TARGET GLEW::glew_s)
    set(GLEW_LIB GLEW::glew_s)
elseif(TARGET glew_s)
    set(GLEW_LIB glew_s)
else()
    message(FATAL_ERROR "Cannot find a GLEW static target. Check glew-cmake version.")
endif()
message(STATUS "[TitanEngine] GLEW target: ${GLEW_LIB}")

target_link_libraries(TitanEngine PUBLIC
    glfw
    ${GLEW_LIB}
    OpenGL::GL
    glm::glm
)

if(WIN32)
    target_link_libraries(TitanEngine PUBLIC opengl32)
endif()

# ── HousingApp static library ─────────────────────────────────────────────────
add_library(HousingApp STATIC
    App/src/Layers/HousingViewerLayer.cpp
)

target_include_directories(HousingApp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/App/src
)

target_link_libraries(HousingApp PUBLIC TitanEngine)

# ── TitanEditor executable ────────────────────────────────────────────────────
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Editor/src/Main.cpp")
    message(FATAL_ERROR "\n  MISSING: Editor/src/Main.cpp\n  Create this file then re-run cmake.\n")
endif()

add_executable(TitanEditor
    Editor/src/Main.cpp
)

target_include_directories(TitanEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/App/src
)

target_link_libraries(TitanEditor PRIVATE HousingApp)

# ── Summary ───────────────────────────────────────────────────────────────────
message(STATUS "")
message(STATUS "=== TitanEngine ===")
message(STATUS "  GLFW  : ${glfw_SOURCE_DIR}")
message(STATUS "  GLEW  : ${glew_SOURCE_DIR}")
message(STATUS "  GLM   : ${glm_SOURCE_DIR}")
message(STATUS "  ImGui : ${imgui_SOURCE_DIR}")
message(STATUS "===================")