cmake_minimum_required(VERSION 3.15)
project(MyProject)

set(CMAKE_CXX_STANDARD 23)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Dependencies
include(Dependencies.cmake)

set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR} CACHE INTERNAL "ImGui include path")
add_subdirectory(external/rlImGui)

set(IMGUI_SOURCES
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

# Projects
add_subdirectory(Core)
add_subdirectory(App)
