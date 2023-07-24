cmake_minimum_required(VERSION 3.11)
project(YA2DE LANGUAGES CXX C VERSION "0.1.1")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# if no -DCMAKE_BUILD_TYPE flag was passed default to debug
if (NOT CMAKE_BUILD_TYPE)
    MESSAGE("Build type not set. Defaulting to DEBUG")
    set(CMAKE_BUILD_TYPE DEBUG)
else ()
    MESSAGE("Build type set to ${CMAKE_BUILD_TYPE}")
endif ()

# Include the FetchContent module
include(FetchContent)

# Fetch SFML
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.0
)

FetchContent_GetProperties(SFML)
if (NOT SFML_POPULATED)
    FetchContent_Populate(SFML)
    set(SFML_BUILD_STATIC_LIBS TRUE)  # Build SFML as static libraries
    set(SFML_INSTALL_PKGCONFIG_FILES FALSE)  # Disable generating pkg-config files
    set(BUILD_SHARED_LIBS FALSE CACHE BOOL "" FORCE)
    set(SFML_BUILD_EXAMPLES FALSE CACHE BOOL "" FORCE)
    set(SFML_BUILD_TESTS FALSE CACHE BOOL "" FORCE)
    set(SFML_INSTALL_DOCS FALSE CACHE BOOL "" FORCE)
    add_subdirectory(${sfml_SOURCE_DIR} ${sfml_BINARY_DIR})
endif ()

# Fetch and include ImGui
FetchContent_Declare(
        ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.89.7
)

FetchContent_GetProperties(ImGui)
if (NOT ImGui_POPULATED)
    FetchContent_Populate(ImGui)
    set(IMGUI_DIR ${imgui_SOURCE_DIR})  # Set ImGui directory

    # Add ImGui source files to your project
    add_library(imgui STATIC
            ${IMGUI_DIR}/imgui.cpp
            ${IMGUI_DIR}/imgui_demo.cpp
            ${IMGUI_DIR}/imgui_draw.cpp
            ${IMGUI_DIR}/imgui_widgets.cpp
            )

    target_include_directories(imgui PUBLIC ${IMGUI_DIR})

    # Enable some compilation options for ImGui
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
endif ()


# Fetch and include ImGui-SFML
FetchContent_Declare(
        ImGui-SFML
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG 2.6.x
)

FetchContent_GetProperties(ImGui-SFML)
if (NOT ImGui-SFML_POPULATED)
    FetchContent_Populate(ImGui-SFML)
    set(IMGUI_SFML_FIND_SFML OFF)
    set(IMGUI_SFML_BUILD_EXAMPLES OFF CACHE INTERNAL "")
    add_subdirectory(${imgui-sfml_SOURCE_DIR} ${imgui-sfml_BINARY_DIR})
endif ()

# Fetch and include Lua
FetchContent_Declare(
        Lua
        GIT_REPOSITORY https://github.com/lua/lua.git
        GIT_TAG v5.4.3
)

FetchContent_GetProperties(Lua)
if (NOT Lua_POPULATED)
    FetchContent_Populate(Lua)
    set(LUA_DIR ${lua_SOURCE_DIR})  # Set Lua directory

    # Add Lua source files to your project
    add_library(lua STATIC
            ${LUA_DIR}/lapi.c
            ${LUA_DIR}/lcode.c
            ${LUA_DIR}/lctype.c
            ${LUA_DIR}/ldebug.c
            ${LUA_DIR}/ldo.c
            ${LUA_DIR}/ldump.c
            ${LUA_DIR}/lfunc.c
            ${LUA_DIR}/lgc.c
            ${LUA_DIR}/llex.c
            ${LUA_DIR}/lmem.c
            ${LUA_DIR}/lobject.c
            ${LUA_DIR}/lopcodes.c
            ${LUA_DIR}/lparser.c
            ${LUA_DIR}/lstate.c
            ${LUA_DIR}/lstring.c
            ${LUA_DIR}/ltable.c
            ${LUA_DIR}/ltm.c
            ${LUA_DIR}/lundump.c
            ${LUA_DIR}/lvm.c
            ${LUA_DIR}/lzio.c
            ${LUA_DIR}/lauxlib.c
            ${LUA_DIR}/lbaselib.c
            ${LUA_DIR}/lcorolib.c
            ${LUA_DIR}/ldblib.c
            ${LUA_DIR}/liolib.c
            ${LUA_DIR}/lmathlib.c
            ${LUA_DIR}/loslib.c
            ${LUA_DIR}/lstrlib.c
            ${LUA_DIR}/ltablib.c
            ${LUA_DIR}/lutf8lib.c
            ${LUA_DIR}/loadlib.c
            ${LUA_DIR}/linit.c
            )

    target_include_directories(lua PUBLIC ${LUA_DIR})
endif ()

# Fetch and include sol2
FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG v3.3.0
)

FetchContent_GetProperties(sol2)
if (NOT sol2_POPULATED)
    FetchContent_Populate(sol2)
    #set(SOL2_DIR ${sol2_SOURCE_DIR}/single/sol2)  # Set sol2 directory
    set(SOL2_DIR ${sol2_SOURCE_DIR}/include)

    # Add sol2 source files to your project
    add_library(sol2 INTERFACE)

    target_include_directories(sol2 INTERFACE ${SOL2_DIR})
endif ()

# Configure file for automatic macros
configure_file(config.h.in "${CMAKE_SOURCE_DIR}/include/config.h")

# Main project include directories
include_directories(include)

# Add your project's source files
set(SOURCES src/main.cpp src/application.cpp include/application.hpp include/state_frame.hpp include/state_stack.hpp include/resource_pack.hpp include/resource_holder.hpp src/state_stack.cpp src/state_frame.cpp include/game_state.hpp src/game_state.cpp)

# Create an executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML libraries to the target (statically)
target_link_libraries(${PROJECT_NAME} PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        imgui
        ImGui-SFML::ImGui-SFML
        lua
        sol2)
