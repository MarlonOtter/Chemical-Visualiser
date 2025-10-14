# Chemical Visualiser

Visualise chemical structure in 2D and 3D.

This project was originally created for my A-Level Final Project in 2024-2025 however I have since made significant changes to the entire codebase.

## Requirements

CMake should download all the required external libraries automatically on build.

However for cpr (the library that I am using for http requests) meson is required. This requires a seperate setup.

If you have python setup, it can be installed by running:

```Bash
pip install meson
```

### Libaries

-   RayLib - Rendering, input handling, etc.
-   cpr - HTTP Requests
-   nlohmann::json - parsing json data

## Build

```Bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

Alternatively, you can open the folder in Visual Studio and it should be able to be ran as usual (Run button / F5).
