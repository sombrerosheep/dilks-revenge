
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(game C)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(game_description "dilks-revenge")
set(game_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(game_include_dir "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(PROCESSOR_ARCH "x64")

OPTION(DREV_DRAW_BB "All Debug options on" OFF)
if (DREV_DRAW_BB)
  add_compile_definitions(DREV_DRAW_BB)
endif(DREV_DRAW_BB)

# Dependencies
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories("./vendor")

##### Info output #####
message(STATUS "")
message(STATUS "====== ${PROJECT_NAME} (${PROJECT_VERSION}) ======")
message(STATUS "${game_description}")
message(STATUS "")
message(STATUS "Platform:   ${CMAKE_SYSTEM}")
message(STATUS "Compiler:   ${CMAKE_CC_COMPILER}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")
message(STATUS "Project Variables:")
message(STATUS "  Source Dir:           ${game_source_dir}")
message(STATUS "  Include Dir:          ${game_include_dir}")
message(STATUS "")
message(STATUS "Dependencies:")
message(STATUS "  SDL_SOURCE_DIR:       ${SDL2_INCLUDE_DIRS}")
message(STATUS "")
message(STATUS "Flags:")
message(STATUS "  DREV_DRAW_BB:         ${DREV_DRAW_BB}")
message(STATUS "")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
  # add_compile_options(/W4 /WX)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# Add -g for valgrind
add_compile_options(-g)

#### Build ####
file(GLOB game_source
  "${game_source_dir}/[a-z0-9_-]*.c*"
)
add_executable(game ${game_source})

set_target_properties(game PROPERTIES C_STANDARD 11 OUTPUT_NAME "dilks-revenge")
target_link_libraries(game ${SDL2_LIBRARIES} m)
target_include_directories(
  game PUBLIC
  ${game_include_dir}
)
