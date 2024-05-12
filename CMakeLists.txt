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
"src/char_player.cpp"
"src/enemy_dummy.cpp"
"src/cmd_light_atk.cpp"
"src/base/actor.cpp"
"src/base/combatant.cpp"
"src/base/action_command.cpp"
)

set(HEADER_FILES
"include/defaults.h"
"include/game.h"
"include/globals.h"
"include/char_player.h"
"include/enemy_dummy.h"
"include/cmd_light_atk.h"
"include/base/generics.h"
"include/base/actor.h"
"include/base/combatant.h"
"include/base/action_command.h"
)

set(STANDARD -static -static-libgcc -static-libstdc++)

if(WIN32)
  message(STATUS "OS detected: Windows")
  set(RAYLIB -lraylib -lopengl32 -lgdi32 -lwinmm)
elseif(LINUX OR UNIX)
  message(STATUS "OS detected: Linux")
  set(RAYLIB -lraylib -lGL -lm -lpthread -ldl -lrt -lX11)
endif()

add_executable("game" ${SOURCE_FILES} ${HEADER_FILES})

if(${BUILD_DEBUG})
  target_compile_options("game" PRIVATE --debug -O0)
  target_link_libraries("game" PRIVATE ${RAYLIB})
else()
    target_compile_options("game" PRIVATE -O3)
  if(WIN32)
    target_link_options("game" PRIVATE -mwindows)
    target_link_libraries("game" PRIVATE ${STANDARD} ${RAYLIB})
  elseif(LINUX OR UNIX)
    target_link_libraries("game" PRIVATE ${RAYLIB})
  endif()
endif()
