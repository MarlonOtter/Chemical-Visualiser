# Chemical Visualiser

Visualise chemical structure in 2D and 3D.

This project was originally created for my A-Level Final Project in 2024-2025 however I have since made significant changes to the entire codebase.

It works by making requests to the PubChem API to retreive chemical structure data, which is then handled and rendered.

## Requirements

CMake should download all the required external libraries automatically on build.

However for cpr (the library that I am using for http requests) meson is required. This requires a seperate setup.

If you have python setup, it can be installed by running:

```Bash
pip install meson
```

### Libraries

-   RayLib - Rendering, input handling, etc.
-   cpr - HTTP Requests
-   nlohmann::json - parsing json data
-   imgui - UI
-   rlImgui - allow imgui to work with raylib

## Build

```Bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

Alternatively, you can open the folder in Visual Studio and it should be able to be ran as usual (Run button / F5).

NOTE: copy icon.png to the build folder if you want the window to have an icon. It is not required however.
