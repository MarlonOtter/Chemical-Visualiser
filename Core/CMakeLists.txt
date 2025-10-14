# Core/CMakeLists.txt
file(GLOB_RECURSE CORE_SOURCES "Source/Core/*.cpp" "Source/*.c")
file(GLOB_RECURSE CORE_HEADERS "Source/Core/*.h" "Source/*.hpp")

add_library(Core STATIC ${CORE_SOURCES} ${CORE_HEADERS} "Source/Core/Layer.h" "Source/Core/Event.h" "Source/Core/Window.h" "Source/Core/Window.cpp" "Source/Core/Layer.cpp" "Source/Core/Json.h")

target_sources(Core PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})



target_link_libraries(Core PUBLIC raylib)
target_link_libraries(Core PUBLIC nlohmann_json::nlohmann_json)

target_include_directories(Core PUBLIC "Source")


