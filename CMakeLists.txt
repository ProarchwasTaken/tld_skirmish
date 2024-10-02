cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER clang++)

project("skirmish" 
VERSION 0.3.0
DESCRIPTION "A short wave based fighter."
HOMEPAGE_URL "https://github.com/ProarchwasTaken/tld_skirmish"
LANGUAGES CXX
)

option(BUILD_DEBUG "Build the game for debugging." OFF)

if(${BUILD_DEBUG})
  set(PROJECT_STAGE "Developer")
  set(COMPILE_OPTIONS --debug -O0)
  set(LINK_OPTIONS "")
else()
  set(PROJECT_STAGE "Beta")
  set(COMPILE_OPTIONS -DNDEBUG)
  set(LINK_OPTIONS -mwindows)
endif()

message(STATUS "Generating build system for: " ${PROJECT_STAGE})

include_directories("include")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH "../")

set(RAYLIB -lraylib -lopengl32 -lgdi32 -lwinmm)
set(META_DATA 
-DVERSION="${CMAKE_PROJECT_VERSION}" 
-DVER_STAGE="${PROJECT_STAGE}"
-DAUTHOR="Tyler Dillard"
-DHOME_URL="${CMAKE_PROJECT_HOMEPAGE_URL}"
)

set(SOURCE_FILES 
"src/main.cpp"
"src/globals.cpp"
"src/game.cpp"
"src/system/audio.cpp"
"src/system/sprites.cpp"
"src/system/wave_manager.cpp"
"src/base/actor.cpp"
"src/base/dynamic_actor.cpp"
"src/base/combatant.cpp"
"src/base/action_command.cpp"
"src/base/sub-weapon.cpp"
"src/utils/camera.cpp"
"src/utils/animation.cpp"
"src/utils/enemies.cpp"
"src/utils/dynamic.cpp"
"src/utils/ai_behavior.cpp"
"src/utils/sound.cpp"
"src/utils/sprite.cpp"
"src/utils/stages.cpp"
"src/utils/text.cpp"
"src/utils/menu.cpp"
"src/scenes/scene_title.cpp"
"src/scenes/scene_menu.cpp"
"src/scenes/scene_subweapon.cpp"
"src/scenes/scene_gameplay.cpp"
"src/hud/hud_life.cpp"
"src/hud/hud_morale.cpp"
"src/combatants/char_player.cpp"
"src/combatants/enemy_ghoul.cpp"
"src/combatants/enemy_wretch.cpp"
"src/combatants/enemy_damned.cpp"
"src/sub-weapons/weapon_knife.cpp"
"src/sub-weapons/weapon_ball.cpp"
"src/action_command/cmd_light_atk.cpp"
"src/action_command/cmd_heavy_atk.cpp"
"src/action_command/cmd_guard.cpp"
"src/action_command/cmd_knife_light.cpp"
"src/action_command/cmd_knife_heavy.cpp"
"src/action_command/cmd_ball_light.cpp"
"src/action_command/cmd_ball_heavy.cpp"
"src/action_command/cmd_ghoul_atk.cpp"
"src/action_command/cmd_wretch_atk.cpp"
"src/projectiles/proj_knife.cpp"
"src/projectiles/proj_ball.cpp"
"src/effects/fx_dmg_number.cpp"
)

set(HEADER_FILES
"include/defaults.h"
"include/globals.h"
"include/game.h"
"include/sys_sprites.h"
"include/sys_audio.h"
"include/sys_wave_manager.h"
"include/base/generics.h"
"include/base/scene.h"
"include/base/actor.h"
"include/base/dynamic_actor.h"
"include/base/combatant.h"
"include/base/action_command.h"
"include/base/sub-weapon.h"
"include/data/data_sprites.h"
"include/data/data_sound.h"
"include/data/data_enemy.h"
"include/data/data_weapon.h"
"include/utils_camera.h"
"include/utils_sound.h"
"include/utils_sprite.h"
"include/utils_animation.h"
"include/utils_ai_behavior.h"
"include/utils_enemies.h"
"include/utils_dynamic.h"
"include/utils_stages.h"
"include/utils_text.h"
"include/utils_menu.h"
"include/utils_sequence.h"
"include/scene_title.h"
"include/scene_menu.h"
"include/scene_subweapon.h"
"include/scene_gameplay.h"
"include/hud_life.h"
"include/hud_morale.h"
"include/char_player.h"
"include/enemy_ghoul.h"
"include/enemy_wretch.h"
"include/enemy_damned.h"
"include/weapon_knife.h"
"include/weapon_ball.h"
"include/cmd_light_atk.h"
"include/cmd_heavy_atk.h"
"include/cmd_guard.h"
"include/cmd_knife_light.h"
"include/cmd_knife_heavy.h"
"include/cmd_ball_light.h"
"include/cmd_ball_heavy.h"
"include/cmd_ghoul_atk.h"
"include/cmd_wretch_atk.h"
"include/proj_knife.h"
"include/proj_ball.h"
"include/fx_dmg_number.h"
)

if(${BUILD_DEBUG})
  message(STATUS "Including debug source and headers files.")
  list(APPEND SOURCE_FILES 
  "src/scenes/scene_debug.cpp" 
  "src/combatants/enemy_dummy.cpp"
  )
  list(APPEND HEADER_FILES 
  "include/scene_debug.h"
  "include/enemy_dummy.h"
  )
endif()

add_executable("skirmish" ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries("skirmish" PRIVATE ${RAYLIB})
target_compile_options("skirmish" PRIVATE ${COMPILE_OPTIONS} ${META_DATA})
target_link_options("skirmish" PRIVATE ${LINK_OPTIONS})

