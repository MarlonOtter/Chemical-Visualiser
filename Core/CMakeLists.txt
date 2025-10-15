# Core/CMakeLists.txt
file(GLOB_RECURSE CORE_SOURCES "Source/Core/*.cpp" "Source/*.c")
file(GLOB_RECURSE CORE_HEADERS "Source/Core/*.h" "Source/*.hpp")

add_library(Core STATIC ${CORE_SOURCES} ${CORE_HEADERS} "Source/Core/Layer.h" "Source/Core/Event.h" "Source/Core/Window.h" "Source/Core/Window.cpp" "Source/Core/Layer.cpp" "Source/Core/Json.h"   "Source/Core/Http/HttpClient.h"  "Source/Core/Renderer/Text.h" "Source/Core/Renderer/Text.cpp" "Source/Core/Renderer/Color.h"  "Source/Core/Renderer/Shape.h" "Source/Core/Renderer/Shape.cpp" "Source/Core/Renderer/Model.h" "Source/Core/Renderer/Model.cpp")

target_sources(Core PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

target_link_libraries(Core PUBLIC
  raylib
  nlohmann_json::nlohmann_json
  cpr::cpr
  rlImGui
  imgui
)

target_include_directories(Core PUBLIC
  ${imgui_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/external/rlImGui
  "Source"
)
