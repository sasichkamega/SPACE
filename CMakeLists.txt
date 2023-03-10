cmake_minimum_required(VERSION 3.22.1)
project(SPACE VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(/W3)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "")
endif()

add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

find_package(SFML REQUIRED)
find_package(FLECS REQUIRED)
find_package(SPDLOG REQUIRED)
find_package(GLM REQUIRED)
find_package(BGFX REQUIRED)

add_executable(main
  engine/engine/main.cpp
  engine/engine/setup_spdlog.cpp
  engine/engine/flecs_helpers.cpp

  engine/filesystem-backend-stdlib.cpp
  engine/filesystem.cpp
  engine/geometry.cpp
  engine/glm-reflection.cpp
  #engine/modules/graphics-backend-sfml.cpp
  engine/graphics.cpp
  engine/transform.cpp
  engine/window-backend-sfml.cpp
  engine/window.cpp
  #engine/graphics-backend-bgfx.cpp
  engine/bgfx.cpp
  engine/bgfx/BgfxLock.cpp
  engine/bgfx/cvars.cpp
  engine/bgfx/internal_components.cpp
  engine/config.cpp
  engine/config2.cpp

)

target_link_libraries(main PRIVATE
  SFML::SFML
  flecs::flecs_static
  spdlog::spdlog
  glm_libs
  bgfx
)

add_executable(game
  game/main.cpp
  game/powernet.cpp
  engine/engine/setup_spdlog.cpp
  game/atmospherics.cpp
)
target_link_libraries(game PRIVATE flecs::flecs_static spdlog::spdlog)