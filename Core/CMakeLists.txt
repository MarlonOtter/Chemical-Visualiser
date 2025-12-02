# Core/CMakeLists.txt
file(GLOB_RECURSE CORE_SOURCES "Source/Core/*.cpp" "Source/*.c")
file(GLOB_RECURSE CORE_HEADERS "Source/Core/*.h" "Source/*.hpp")

add_library(Core STATIC ${CORE_SOURCES} ${CORE_HEADERS} "Source/Core/Layer.h" "Source/Core/Event.h" "Source/Core/Window.h" "Source/Core/Window.cpp" "Source/Core/Layer.cpp" "Source/Core/Json.h"   "Source/Core/Http/HttpClient.h"  "Source/Core/Renderer/Text.h" "Source/Core/Renderer/Text.cpp" "Source/Core/Renderer/Color.h"  "Source/Core/Renderer/Shape.h" "Source/Core/Renderer/Shape.cpp" "Source/Core/Renderer/Model.h" "Source/Core/Renderer/Model.cpp" "Source/Core/Utils/String.h" "Source/Core/Utils/String.cpp" "Source/Core/Cameras/ArcBall.h" "Source/Core/Cameras/ArcBall.cpp" "Source/Core/Utils/Inputs.h"  "Source/Core/Math/Interpolation.h" "Source/Core/Math/Math.h" "Source/Core/System/FileDialog.h" "Source/Core/System/FileDialog.cpp" "Source/Core/System/NfdTranslation.h" "Source/Core/System/FileDialogTypes.h")

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${CORE_SOURCES})

target_link_libraries(Core PUBLIC
  raylib
  nlohmann_json::nlohmann_json
  cpr::cpr
  rlImGui
  imgui
)
 
target_link_libraries(Core PRIVATE nfd)

target_include_directories(Core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/external/rlImGui
  "Source"
)


