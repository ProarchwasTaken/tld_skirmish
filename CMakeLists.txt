cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER clang++)

project("skirmish" 
VERSION 1.0.1
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
  set(PROJECT_STAGE "Release")
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

include_directories("triblib/include")
add_subdirectory("triblib")

set(SOURCE_FILES 
"src/main.cpp"
"src/globals.cpp"
"src/game.cpp"
"src/system/sprites.cpp"
"src/system/wave_manager.cpp"
"src/system/transition.cpp"
"src/base/combatant.cpp"
"src/base/action_command.cpp"
"src/base/sub-weapon.cpp"
"src/utils/settings.cpp"
"src/utils/camera.cpp"
"src/utils/enemies.cpp"
"src/utils/ai_behavior.cpp"
"src/utils/stages.cpp"
"src/utils/menu.cpp"
"src/scenes/scene_splash.cpp"
"src/scenes/scene_title.cpp"
"src/scenes/scene_menu.cpp"
"src/scenes/scene_subweapon.cpp"
"src/scenes/scene_settings.cpp"
"src/scenes/scene_control.cpp"
"src/scenes/scene_index.cpp"
"src/scenes/scene_pregame.cpp"
"src/scenes/scene_gameplay.cpp"
"src/scenes/scene_win.cpp"
"src/scenes/scene_gameover.cpp"
"src/hud/hud_life.cpp"
"src/hud/hud_morale.cpp"
"src/hud/hud_menu.cpp"
"src/hud/hud_menu_btn.cpp"
"src/hud/hud_pause.cpp"
"src/combatants/char_player.cpp"
"src/combatants/enemy_ghoul.cpp"
"src/combatants/enemy_wretch.cpp"
"src/combatants/enemy_damned.cpp"
"src/sub-weapons/weapon_knife.cpp"
"src/sub-weapons/weapon_ball.cpp"
"src/sub-weapons/weapon_gun.cpp"
"src/action_command/cmd_light_atk.cpp"
"src/action_command/cmd_heavy_atk.cpp"
"src/action_command/cmd_guard.cpp"
"src/action_command/cmd_knife_light.cpp"
"src/action_command/cmd_knife_heavy.cpp"
"src/action_command/cmd_ball_light.cpp"
"src/action_command/cmd_ball_heavy.cpp"
"src/action_command/cmd_gun_light.cpp"
"src/action_command/cmd_gun_heavy.cpp"
"src/action_command/cmd_ghoul_atk.cpp"
"src/action_command/cmd_wretch_atk.cpp"
"src/action_command/cmd_damned_grb.cpp"
"src/projectiles/proj_knife.cpp"
"src/projectiles/proj_ball.cpp"
"src/effects/fx_dmg_number.cpp"
"src/effects/fx_gunprobe.cpp"
"src/effects/fx_gunbolt.cpp"
)

set(HEADER_FILES
"include/defaults.h"
"include/globals.h"
"include/game.h"
"include/system/sprites.h"
"include/system/wave_manager.h"
"include/system/transition.h"
"include/base/combatant.h"
"include/base/action_command.h"
"include/base/sub-weapon.h"
"include/data/data_enemy.h"
"include/data/data_weapon.h"
"include/data/data_index.h"
"include/utils/settings.h"
"include/utils/camera.h"
"include/utils/ai_behavior.h"
"include/utils/enemies.h"
"include/utils/stages.h"
"include/utils/menu.h"
"include/utils/sequence.h"
"include/scenes/scene_splash.h"
"include/scenes/scene_title.h"
"include/scenes/scene_menu.h"
"include/scenes/scene_subweapon.h"
"include/scenes/scene_settings.h"
"include/scenes/scene_control.h"
"include/scenes/scene_index.h"
"include/scenes/scene_pregame.h"
"include/scenes/scene_gameplay.h"
"include/scenes/scene_win.h"
"include/scenes/scene_gameover.h"
"include/hud/hud_life.h"
"include/hud/hud_morale.h"
"include/hud/hud_menu.h"
"include/hud/hud_menu_btn.h"
"include/hud/hud_pause.h"
"include/combatants/char_player.h"
"include/combatants/enemy_ghoul.h"
"include/combatants/enemy_wretch.h"
"include/combatants/enemy_damned.h"
"include/sub-weapons/weapon_knife.h"
"include/sub-weapons/weapon_ball.h"
"include/sub-weapons/weapon_gun.h"
"include/action_command/cmd_light_atk.h"
"include/action_command/cmd_heavy_atk.h"
"include/action_command/cmd_guard.h"
"include/action_command/cmd_knife_light.h"
"include/action_command/cmd_knife_heavy.h"
"include/action_command/cmd_ball_light.h"
"include/action_command/cmd_ball_heavy.h"
"include/action_command/cmd_gun_light.h"
"include/action_command/cmd_gun_heavy.h"
"include/action_command/cmd_ghoul_atk.h"
"include/action_command/cmd_wretch_atk.h"
"include/action_command/cmd_damned_grb.h"
"include/projectiles/proj_knife.h"
"include/projectiles/proj_ball.h"
"include/effects/fx_dmg_number.h"
"include/effects/fx_gunprobe.h"
"include/effects/fx_gunbolt.h"
)

if(${BUILD_DEBUG})
  message(STATUS "Including debug source and headers files.")
  list(APPEND SOURCE_FILES 
  "src/scenes/scene_debug.cpp" 
  "src/combatants/enemy_dummy.cpp"
  )
  list(APPEND HEADER_FILES 
  "include/scenes/scene_debug.h"
  "include/combatants/enemy_dummy.h"
  )
endif()

add_executable("skirmish" ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries("skirmish" PRIVATE ${RAYLIB})
target_link_libraries("skirmish" PRIVATE "triblib")
target_compile_options("skirmish" PRIVATE ${COMPILE_OPTIONS} ${META_DATA})
target_link_options("skirmish" PRIVATE ${LINK_OPTIONS})

