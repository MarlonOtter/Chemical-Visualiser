# Core/CMakeLists.txt
file(GLOB_RECURSE CORE_SOURCES "Source/Core/*.cpp" "Source/*.c")
file(GLOB_RECURSE CORE_HEADERS "Source/Core/*.h" "Source/*.hpp")

add_library(Core STATIC ${CORE_SOURCES} ${CORE_HEADERS} "Source/Core/Layer.h" "Source/Core/Event.h" "Source/Core/Window.h" "Source/Core/Window.cpp")

target_sources(Core PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})



target_link_libraries(Core raylib)

target_include_directories(Core PUBLIC "Source" "vendor/stb")
