include(FetchContent)

# raylib - prefer system package, otherwise fetch
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
  endif()
endif()

# nlohmann::json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_GetProperties(json)
if (NOT json_POPULATED)
  FetchContent_MakeAvailable(json)
endif()

# cpr
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.12.0
)
FetchContent_GetProperties(cpr)
if (NOT cpr_POPULATED)
  FetchContent_MakeAvailable(cpr)
endif()

# ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG docking
)
FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
  FetchContent_MakeAvailable(imgui)
endif()

# nfd
FetchContent_Declare(
  nfd
  GIT_REPOSITORY https://github.com/MarlonOtter/nativefiledialog-extended.git
  GIT_TAG master
)
FetchContent_GetProperties(nfd)
if (NOT nfd_POPULATED)
  FetchContent_MakeAvailable(nfd)
endif()