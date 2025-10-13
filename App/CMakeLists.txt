# App/CMakeLists.txt
file(GLOB_RECURSE APP_SOURCES "Source/*.cpp" "Source/*.c")
file(GLOB_RECURSE APP_HEADERS "Source/*.h" "Source/*.hpp")

add_executable(App ${APP_SOURCES} ${APP_HEADERS})

target_include_directories(App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# Link against Core
target_link_libraries(App PRIVATE Core)