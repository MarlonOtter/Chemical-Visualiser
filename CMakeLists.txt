cmake_minimum_required(VERSION 3.15)
project(ChemicalVisualiser 
    LANGUAGES C CXX
    DESCRIPTION "Chemical Visualiser"
    VERSION 0.1.0
)

# Enforce C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Override Shader Source Directory
set(SLATE_SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/Slate/src/Shaders" CACHE PATH "" FORCE)

# Build projects
add_subdirectory(ChemicalVisualiser)
add_subdirectory(external/Slate)
add_subdirectory(external/Slate-Http)
add_subdirectory(external/Slate-Assets)
