# Core/CMakeLists.txt
file(GLOB_RECURSE CORE_SOURCES "Source/Core/*.cpp" "Source/*.c")
file(GLOB_RECURSE CORE_HEADERS "Source/Core/*.h" "Source/*.hpp")

add_library(Core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)