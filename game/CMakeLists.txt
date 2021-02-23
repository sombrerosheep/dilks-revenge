cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(game C)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(game_description "game")
set(game_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(game_include_dir "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(PROCESSOR_ARCH "x64")

# Dependencies
set(SDL_SOURCE "$ENV{sdl_src_dir}/")
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
add_subdirectory(${SDL_SOURCE} "${SDL_SOURCE}/build")


##### Info output #####
message(STATUS "")
message(STATUS "====== ${PROJECT_NAME} (${PROJECT_VERSION}) ======")
message(STATUS "${game_description}")
message(STATUS "")
message(STATUS "Platform:   ${CMAKE_SYSTEM}")
message(STATUS "Compiler:   ${CMAKE_CXX_COMPILER}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS "Project Variables:")
message(STATUS "  Source Dir:           ${game_source_dir}")
message(STATUS "  Include Dir:          ${game_include_dir}")
message(STATUS "")
message(STATUS "Dependencies:")
message(STATUS "  SDL_SOURCE_DIR:       ${SDL_SOURCE}")
message(STATUS "")

if (MSVC)
  add_compile_options(/W4 /WX)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

#### Build ####
file(GLOB game_source
"${game_source_dir}/[a-z0-9_-]*.c*"
)
add_executable(game ${game_source})

set_target_properties(game PROPERTIES C_STANDARD 99)
target_link_libraries(game SDL2-static SDL2main)
target_include_directories(
  game PUBLIC
  ${game_include_dir}
  "${SDL_SOURCE}/include"
)
