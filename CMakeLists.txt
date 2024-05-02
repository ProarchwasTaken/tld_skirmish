cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

option(BUILD_DEBUG "Build the game for debugging." OFF)

if(${BUILD_DEBUG})
  message(STATUS "Building the game for debugging.")
  set(CMAKE_CXX_COMPILER clang++)
else()
  message(STATUS "Building the game for release.")
  set(CMAKE_CXX_COMPILER g++)
endif()

project("game" 
VERSION 0.0.1
DESCRIPTION "A short wave based fighter."
LANGUAGES CXX
)

include_directories("include")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH "../")

set(SOURCE_FILES 
"src/main.cpp"
"src/game.cpp"
)

set(HEADER_FILES
"include/defaults.h"
"include/game.h"
)

set(STANDARD -static -static-libgcc -static-libstdc++)
set(RAYLIB -lraylib -lopengl32 -lgdi32 -lwinmm)

add_executable("game" ${SOURCE_FILES} ${HEADER_FILES})

if(${BUILD_DEBUG})
  target_compile_options("game" PRIVATE --debug -O0)
  target_link_libraries("game" PRIVATE ${RAYLIB})
else()
  target_link_options("game" PRIVATE -mwindows)
  target_compile_options("game" PRIVATE -O3)
  target_link_libraries("game" PRIVATE ${STANDARD} ${RAYLIB})
endif()
