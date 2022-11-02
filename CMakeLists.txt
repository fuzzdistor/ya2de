cmake_minimum_required(VERSION 3.16)
project(YA2DE LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5.1 REQUIRED graphics window system audio)

include_directories(
  "${CMAKE_SOURCE_DIR}/include"
  "${CMAKE_SOURCE_DIR}/extlibs/LoggerCpp/include"
  "${CMAKE_SOURCE_DIR}/extlibs/Sol2/include"
  "${CMAKE_SOURCE_DIR}/extlibs/lua-5.4.4/src"
  "${CMAKE_SOURCE_DIR}/extlibs/nlohmann-json/include"
  ${SFML_INCLUDE_DIR}
)

add_subdirectory("${CMAKE_SOURCE_DIR}/extlibs/LoggerCpp")
add_subdirectory("${CMAKE_SOURCE_DIR}/extlibs/Sol2")
add_subdirectory("${CMAKE_SOURCE_DIR}/extlibs/nlohmann-json")

add_executable("${CMAKE_PROJECT_NAME}"
 src/areaswitchnode.cpp
 src/main.cpp
 src/game.cpp
 src/nodefactories.cpp
 src/scene.cpp
 src/scenedata.cpp
 src/scenenode.cpp
 src/shapenode.cpp
 src/soundplayernode.cpp
 src/spritenode.cpp
 src/textnode.cpp
 src/textboxnode.cpp
 src/tilemapnode.cpp
 src/tileset.cpp
 src/triggernode.cpp
 src/ysortnode.cpp
 include/areaswitchnode.hpp
 include/init.hpp
 include/input.hpp
 include/game.hpp
 include/nodefactories.hpp
 include/scene.hpp
 include/scenedata.hpp
 include/resourcecollection.hpp
 include/resourcecollection.inl
 include/resourceidentifiers.hpp
 include/scenenode.hpp
 include/shapenode.hpp
 include/soundplayernode.hpp
 include/spritenode.hpp
 include/textnode.hpp
 include/textboxnode.hpp
 include/tilemapnode.hpp
 include/tileset.hpp
 include/triggernode.hpp
 include/ysortnode.hpp
)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE
      -Wall
      -Wextra
      -Wshadow
      -Wnon-virtual-dtor
      -pedantic
      -Wold-style-cast
      -Wcast-align
      -Wunused
      -Woverloaded-virtual
      -Wpedantic
      -Wconversion
      -Wsign-conversion
      -Wmisleading-indentation
      -Wduplicated-cond
      -Wduplicated-branches
      -Wlogical-op
      -Wnull-dereference
      -Wuseless-cast
      -Wdouble-promotion
      -Wformat=2
    )
endif(CMAKE_COMPILER_IS_GNUCXX)

set_target_properties(nlohmann-json PROPERTIES LINKER_LANGUAGE CXX)

if(CMAKE_BUILD_TYPE MATCHES RELEASE)
    target_link_libraries("${CMAKE_PROJECT_NAME}" ${SFML_LIBRARIES} sfml-graphics sfml-window sfml-system sfml-audio LoggerCpp sol2 lua)
endif()
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    target_link_libraries("${CMAKE_PROJECT_NAME}" ${SFML_LIBRARIES} sfml-graphics-d sfml-window-d sfml-system-d sfml-audio-d LoggerCpp sol2 lua)
endif()

