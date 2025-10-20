# App/CMakeLists.txt
file(GLOB_RECURSE APP_SOURCES "Source/*.cpp" "Source/*.c")
file(GLOB_RECURSE APP_HEADERS "Source/*.h" "Source/*.hpp")

add_executable(App ${APP_SOURCES} ${APP_HEADERS} "Source/Layers/AppLayer.cpp" "Source/Layers/View2DLayer.h" "Source/Layers/View3DLayer.h" "Source/Layers/View2DLayer.cpp" "Source/Layers/View3DLayer.cpp"   "Source/ChemVis/Chemical.h" "Source/ChemVis/Chemical.cpp" "Source/Layers/InterfaceLayer.h" "Source/WindowData.h" "Source/ChemVis/PubChem.h" "Source/ChemVis/PubChem.cpp")


target_include_directories(App PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# Link against Core
target_link_libraries(App PRIVATE Core)