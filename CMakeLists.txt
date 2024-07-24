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

project("skirmish" 
VERSION 0.0.5
DESCRIPTION "A short wave based fighter."
LANGUAGES CXX
)

include_directories("include")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH "../")

set(SOURCE_FILES 
"src/main.cpp"
"src/globals.cpp"
"src/game.cpp"
"src/sprite_loader.cpp"
"src/audio.cpp"
"src/base/actor.cpp"
"src/base/combatant.cpp"
"src/base/action_command.cpp"
"src/base/sub-weapon.cpp"
"src/utils/camera.cpp"
"src/utils/animation.cpp"
"src/utils/enemies.cpp"
"src/utils/ai_behavior.cpp"
"src/utils/sound.cpp"
"src/utils/sprite.cpp"
"src/utils/stages.cpp"
"src/utils/text.cpp"
"src/utils/menu.cpp"
"src/wave_manager.cpp"
"src/scenes/scene_title.cpp"
"src/scenes/scene_menu.cpp"
"src/scenes/scene_gameplay.cpp"
"src/scenes/scene_debug.cpp"
"src/hud/hud_life.cpp"
"src/hud/hud_morale.cpp"
"src/combatants/char_player.cpp"
"src/combatants/enemy_dummy.cpp"
"src/combatants/enemy_ghoul.cpp"
"src/action_command/cmd_ghoul_atk.cpp"
"src/action_command/cmd_light_atk.cpp"
"src/action_command/cmd_heavy_atk.cpp"
"src/action_command/cmd_guard.cpp"
)

set(HEADER_FILES
"include/defaults.h"
"include/globals.h"
"include/game.h"
"include/utils.h"
"include/sprite_loader.h"
"include/audio.h"
"include/base/generics.h"
"include/base/scene.h"
"include/base/actor.h"
"include/base/combatant.h"
"include/base/action_command.h"
"include/base/sub-weapon.h"
"include/wave_manager.h"
"include/scene_title.h"
"include/scene_menu.h"
"include/scene_gameplay.h"
"include/scene_debug.h"
"include/hud_life.h"
"include/hud_morale.h"
"include/char_player.h"
"include/enemy_dummy.h"
"include/enemy_ghoul.h"
"include/cmd_ghoul_atk.h"
"include/cmd_light_atk.h"
"include/cmd_heavy_atk.h"
"include/cmd_guard.h"
)

set(STANDARD -static -static-libgcc -static-libstdc++)

if(WIN32)
  message(STATUS "OS detected: Windows")
  set(RAYLIB -lraylib -lopengl32 -lgdi32 -lwinmm)
elseif(LINUX OR UNIX)
  message(STATUS "OS detected: Linux")
  set(RAYLIB -lraylib -lGL -lm -lpthread -ldl -lrt -lX11)
endif()

add_executable("skirmish" ${SOURCE_FILES} ${HEADER_FILES})

if(${BUILD_DEBUG})
  target_compile_options("skirmish" PRIVATE --debug -O0)
  target_link_libraries("skirmish" PRIVATE ${RAYLIB})
else()
    target_compile_options("skirmish" PRIVATE -O3)
  if(WIN32)
    target_link_options("skirmish" PRIVATE -mwindows)
    target_link_libraries("skirmish" PRIVATE ${STANDARD} ${RAYLIB})
  elseif(LINUX OR UNIX)
    target_link_libraries("skirmish" PRIVATE ${RAYLIB})
  endif()
endif()
