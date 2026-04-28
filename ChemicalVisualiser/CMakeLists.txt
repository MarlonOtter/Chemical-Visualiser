# App/CMakeLists.txt
file(GLOB_RECURSE APP_SOURCES "Source/*.cpp" "Source/*.c")
file(GLOB_RECURSE APP_HEADERS "Source/*.h" "Source/*.hpp")

add_executable(ChemicalVisualiser  
    "src/ChemicalVisualiserApp.cpp"
    "src/ChemicalVisualiser/InterfaceLayer.h" 
    "src/WindowData.h" 
    "src/ChemicalVisualiser/Settings.h" 
    "src/ChemicalVisualiser/Settings.cpp"
    "src/Camera2D.h"
    "src/Camera2D.cpp"
    "src/ArcballCamera.h" 
    "src/ArcballCamera.cpp" 
    "src/ChemicalVisualiser/ChemicalAsset.h" 
    "src/ChemicalVisualiser/ChemicalVisualiserLayer.h" 
    "src/ChemicalVisualiser/ChemicalVisualiserLayer.cpp" 
    "src/ChemicalVisualiser/ChemicalSource.h" 
    "src/ChemicalVisualiser/ChemicalSource.cpp"
    "src/ChemicalVisualiser/ChemicalAsset.cpp" 
    "src/ChemicalVisualiser/Chemical.h" 
    "src/ChemicalVisualiser/Chemical.cpp"
    "src/ChemicalVisualiser/PubChem.h"
    "src/ChemicalVisualiser/PubChem.cpp"
    "src/ChemicalVisualiser/InterfaceLayer.h"
    "src/ChemicalVisualiser/InterfaceLayer.cpp"
    "src/ChemicalVisualiser/ChemicalList.h"
    "src/ChemicalVisualiser/ChemicalList.cpp"
 "src/ChemicalVisualiser/StartupScreenLayer.h")

target_include_directories(ChemicalVisualiser PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set_target_properties(ChemicalVisualiser PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

if(WIN32)
    # add icon to .exe
    enable_language(RC)
    target_sources(ChemicalVisualiser PRIVATE app_icon.rc)
    
    # remove terminal in release build
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(ChemicalVisualiser PROPERTIES
            WIN32_EXECUTABLE TRUE
            LINK_FLAGS "/ENTRY:mainCRTStartup"
        )
    endif()
endif()

# Link against Core
target_link_libraries(ChemicalVisualiser 
    PRIVATE Slate::Core
    PRIVATE Slate::Http
    PRIVATE Slate::Assets
)