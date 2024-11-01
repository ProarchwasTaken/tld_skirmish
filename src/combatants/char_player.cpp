// combatants/char_player.cpp
#include <cstdint>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <random>
#include <cassert>
#include "globals.h"
#include "base/generics.h"
#include "base/combatant.h"
#include "base/action_command.h"
#include "utils_animation.h"
#include "utils_camera.h"
#include "utils_sound.h"
#include "scene_gameplay.h"
#include "cmd_light_atk.h"
#include "cmd_heavy_atk.h"
#include "cmd_guard.h"
#include "weapon_knife.h"
#include "weapon_ball.h"
#include "weapon_gun.h"
#include "char_player.h"
#include <plog/Log.h>

using std::make_unique, std::unique_ptr, std::uniform_real_distribution;


PlayerCharacter::PlayerCharacter(combatant_list &enemies, uint8_t &phase):
  Combatant("Player", TYPE_PLAYER, PLR_HP, PLR_STABILITY, PLR_START_POS, 
            PLR_HITBOX_SCALE, {64, 64}, PLR_HITBOX_OFFSET)
{
  PLOGI << "Initializing the player character.";
  current_sprite = sprites::player[1];
  game_phase = &phase;

  morale = 10;
  max_morale = 50;

  anim_walk = {1, 2, 3, 2};
  walk_frametime = 0.15;

  anim_death = {15, 16};
  death_frametime = 0.5;

  anim_endure = {15, 17, 18};
  endure_frametime = 1.0;

  movement_speed = 1.75;
  regen_time = 2;

  buf_clear_time = 0.010;

  this->enemies = &enemies;
  PLOGI << "Player initialization complete.";
}

PlayerCharacter::~PlayerCharacter() {
  PLOGI << "Deleting the player character from memory.";
  input_buffer.clear();
  sub_weapon.reset();
}

void PlayerCharacter::assignSubWeapon(uint8_t weapon_id) {
  PLOGI << "Assigning Sub-Weapon associated with id: " << int(weapon_id);
  switch (weapon_id) {
    case WEAPON_KNIFE: {
      sub_weapon.reset();
      sub_weapon = make_unique<WeaponKnife>(this);
      break;
    }
    case WEAPON_BALL: {
      sub_weapon.reset();
      sub_weapon = make_unique<WeaponBall>(this);
      break;
    }
    case WEAPON_GUN: {
      sub_weapon.reset();
      sub_weapon = make_unique<WeaponGun>(this);
      break;
    }
    default: {
      PLOGE << "Invalid Weapon ID!";
    }
  }

  assert(sub_weapon != nullptr && "Invalid Weapon ID!");
}

void PlayerCharacter::update() {
  assert(sub_weapon != nullptr && "You forgot to call assignSubWeapon()");
  bufferTimerCheck();

  if (*game_phase == PHASE_REST) {
    lifeRegen();
  }

  switch (state) {
    case NEUTRAL: {
      stabilityRegen();

      moving = isMoving();
      updateDirection();
      movement(movement_speed, false);

      interpretBuffer();
      break;
    }
    case HIT_STUN: {
      applyKnockback(PLR_BOUNDS);
      stunSequence();
      break;
    }
    case DEAD: {
      if (endure) {
        endureSequence();
      }
      else {
        deathSequence(sprites::player, anim_death, death_frametime);
      }
      break;
    }
    default: {
      commandSequence();
      interpretBuffer();
    }
  }

  sub_weapon->update();
  clearBufferCheck();
}

bool PlayerCharacter::isMoving() {
  if (moving_left != moving_right) {
    Animation::play(this, sprites::player, anim_walk, walk_frametime);
    return true;
  }
  else { 
    useNeutralSprite();
    return false;
  }
}

bool PlayerCharacter::isMoving(std::vector<int> &frame_order, 
                               const float frame_time, 
                               const bool custom_neutral, 
                               Texture *neutral_sprite)
{
  if (moving_left != moving_right) {
    Animation::play(this, sprites::player, frame_order, frame_time);
    return true;
  }
  else if (custom_neutral == false) {
    useNeutralSprite();
    return false;
  }
  else {
    assert(neutral_sprite != NULL);
    current_sprite = neutral_sprite;
    return false;
  }
}

void PlayerCharacter::useNeutralSprite() {
  switch (*game_phase) {
    case PHASE_REST: {
      current_sprite = sprites::player[0];
      break;
    }
    case PHASE_ACTION: {
      current_sprite = sprites::player[1];
      break;
    }
  }
}

void PlayerCharacter::updateDirection() {
  if (moving_right) {
    direction = RIGHT;
  }
  else if (moving_left) {
    direction = LEFT;
  }
}

void PlayerCharacter::movement(float speed, bool automatic, 
                               int8_t *direction) 
{
  if (!moving && automatic == false) {
    return;
  }

  int8_t move_direction;
  if (direction != NULL) {
    move_direction = *direction;
  }
  else {
    move_direction = this->direction;
  }

  float magnitude = (speed * move_direction) * DELTA_TIME;
  int half_scaleX = hitbox_scale.x / 2;
  float offset = position.x + magnitude + (half_scaleX * move_direction);

  if (offset < -PLR_BOUNDS) {
    position.x = -PLR_BOUNDS + half_scaleX;
  }
  else if (offset > PLR_BOUNDS) {
    position.x = PLR_BOUNDS - half_scaleX;
  }
  else {
    position.x += magnitude;
  }

  hitboxCorrection();
  texRectCorrection();
}

void PlayerCharacter::decelerate(float &percentage, const float time,
                                 const float max_speed)
{
  if (percentage == 0.0) {
    return;
  }

  const float speed = Lerp(0, max_speed, percentage);
  movement(speed, true);

  percentage -= GetFrameTime() / time;
  percentage = Clamp(percentage, 0, max_speed);
}

void PlayerCharacter::bufferTimerCheck() {
  buf_empty = input_buffer.size() == 0;

  bool detected_first_input = !buf_empty && buf_timer_started == false;
  if (detected_first_input) {
    PLOGI << "Detected first input in the buffer. Starting timer.";
    buf_input_timestamp = CURRENT_TIME;
    buf_timer_started = true;
  }
}

void PlayerCharacter::interpretBuffer() {
  if (buf_timer_started == false) {
    return;
  }

  float time_elapsed = CURRENT_TIME - buf_input_timestamp;
  bool about_to_clear = time_elapsed >= buf_clear_time;

  if (about_to_clear == false) {
    return;
  }

  if (state == NEUTRAL) {
    PLOGI << "The player is in the NEUTRAL state. Using normal interpret" 
    " logic.";
    normalInterpretLogic();
  }
  else {
    PLOGI << "The player is not in the NEUTRAL state. Using special"
    " interpret logic based on the assumption that the player is using"
    " an action command.";
    specialInterpretLogic();
  }
}

void PlayerCharacter::clearBufferCheck() {
  if (buf_timer_started == false) {
    return;
  }

  float time_elapsed = CURRENT_TIME - buf_input_timestamp;
  if (time_elapsed >= buf_clear_time) {
    PLOGD << "Clearing input buffer.";
    input_buffer.clear();
    buf_timer_started = false;
  }
}

void PlayerCharacter::normalInterpretLogic() {
  uint8_t first_input = input_buffer.front();
  unique_ptr<ActionCommand> command = nullptr;

  PLOGI << "Deciding what commands to use depending on input buffer.";
  switch (first_input) {
    case BTN_LIGHT_ATK: {
      PLOGI << "Attempting to assign LightAttack";
      command = make_unique<LightAttack>(this);
      break;
    }
    case BTN_HEAVY_ATK: {
      PLOGI << "Attempting to assign HeavyAttack";
      command = make_unique<HeavyAttack>(this);
      break;
    }
    case BTN_LIGHT_TECH: {
      PLOGI << "Attempting to assign light weapon technique.";
      command = sub_weapon->lightTechnique();
      break;
    }
    case BTN_HEAVY_TECH: {
      PLOGI << "Attempting to assign heavy weapon technique";
      command = sub_weapon->heavyTechnique();
      break;
    }
    case BTN_GUARD: {
      PLOGI << "Attempting to assign Guard";
      command = make_unique<Guard>(this, sprites::plr_metadata, 
                                   PLR_BOUNDS, true);
      break;
    }
    default: {
      PLOGI << "No valid commands found!";
    }
  }

  if (command != nullptr) {
    useCommand(command);
  }
}

void PlayerCharacter::specialInterpretLogic() {
  assert(current_command != nullptr &&
         "Caught the player with a command assigned to them!");

  if (parried_attack) {
    PLOGI << "Detected that the player has parried an attack. Switching "
      "back to interpret logic.";
    SoundUtils::play("cmd_cancel");
    
    // Best part is that I didn't need to do much to implement this. :)
    normalInterpretLogic();

    invulnerable = false;
    parried_attack = false;
    return;
  }

  PLOGI << "The player is using: " << current_command->command_name;
  switch (current_command->type) {
    case CMD_NORM_LIGHT: {
      lightAttackHandling();
      break;
    }
    case CMD_NORM_HEAVY: {
      heavyAttackHanding();
      break;
    }
    case CMD_TECH_LIGHT: {
      sub_weapon->lightTechHandling();
      break;
    }
    case CMD_TECH_HEAVY: {
      sub_weapon->heavyTechHandling();
      break;
    }
  }
}

void PlayerCharacter::lightAttackHandling() {
  uint8_t first_input = input_buffer.front();
  unique_ptr<ActionCommand> command;

  auto light_atk = static_cast<LightAttack*>(current_command.get());

  if (light_atk->attack_connected == false) {
    return;
  }

  switch (first_input) {
    case BTN_HEAVY_ATK: {
      command = make_unique<HeavyAttack>(this);
      incrementMorale(1);
      break;
    } 
    case BTN_LIGHT_TECH: {
      command = sub_weapon->lightTechnique();
      break;
    }
    case BTN_HEAVY_TECH: {
      command = sub_weapon->heavyTechnique();
      break;
    }
    // Did this for fun. If it's too OP, it would be easy to remove.
    case BTN_GUARD: {
      command = make_unique<Guard>(this, sprites::plr_metadata, 
                                   PLR_BOUNDS, true);
      break;
    }
  }

  if (command != nullptr) {
    SoundUtils::play("cmd_cancel");
    useCommand(command);
  }
}

void PlayerCharacter::heavyAttackHanding() {
  uint8_t first_input = input_buffer.front();
  unique_ptr<ActionCommand> command = nullptr;

  auto heavy_atk = static_cast<HeavyAttack*>(current_command.get());

  if (heavy_atk->attack_connected == false) {
    return;
  } 

  if (first_input == BTN_HEAVY_TECH) {
    command = sub_weapon->heavyTechnique();
  }

  if (command != nullptr) {
    SoundUtils::play("cmd_cancel");
    useCommand(command);
  }
}

void PlayerCharacter::lifeRegen() {
  if (health == max_health) {
    return;
  }

  float time_elapsed = CURRENT_TIME - regen_timestamp;
  if (time_elapsed < regen_time) {
    return;
  }

  health++;
  createDamageNumber(1, COLORS::PALETTE[14]);
  regen_timestamp = CURRENT_TIME;

  if (critical_health) {
    healthCheck();
  }
}

void PlayerCharacter::incrementMorale(uint8_t value) {
  if (morale == max_morale) {
    return;
  }

  int new_morale = morale + value;
  if (new_morale > max_morale) {
    morale = max_morale;
  }
  else {
    morale = new_morale;
  }
}

void PlayerCharacter::takeDamage(uint16_t dmg_magnitude, 
                                 float guard_pierce, 
                                 float stun_time, 
                                 float kb_velocity,
                                 int8_t kb_direction)
{
  bool is_grabbed = this->stun_time >= 10.0;
  if (is_grabbed) {
    PLOGD << "Detected that the player is grabbed.";
    stun_time = 0;
    kb_velocity = 0.0;
    kb_direction = 0.0;
  }

  Combatant::takeDamage(dmg_magnitude, guard_pierce, stun_time, 
                        kb_velocity, kb_direction);

  if (state == HIT_STUN && stun_time != 0) {
    current_sprite = sprites::player[7];
  }

  if (critical_health == false) {
    healthCheck();
  }

  bool fatal_damage = health == 0;
  bool reached_combo_limit = combo > 5;
  if (fatal_damage == false || reached_combo_limit) {
    return;
  }

  float morale_percent = static_cast<float>(morale) / max_morale;
  float endure_chance = morale_percent / (2 - (combo / 5.0));  

  uniform_real_distribution<float> range(0.0, 1.0);
  float random_value = range(RNG::generator);
  bool eligible = dmg_magnitude < (max_health * 2);

  if (eligible && random_value <= endure_chance) {
    endure = true;
  }
}

float PlayerCharacter::getStunTime() {
  return this->stun_time;
}

void PlayerCharacter::healthCheck() {
  float life_percentage = static_cast<float>(health) / max_health;

  if (critical_health == false && life_percentage <= PLR_HP_CRITICAL) {
    PLOGI << "The player is now in critical health!";
    critical_health = true;
    SoundUtils::play("critical_health");
  }
  else if (critical_health && life_percentage > PLR_HP_CRITICAL) {
    PLOGI << "The player has reached safe levels of HP!";
    critical_health = false;
  }
}

void PlayerCharacter::endureSequence() {
  Animation::play(this, sprites::player, anim_endure, endure_frametime,
                  false);

  float time_elapsed = CURRENT_TIME - death_timestamp;
  bool end_of_animation = current_frame == current_anim->end();

  float death_time = endure_frametime * anim_endure.size();

  if (end_of_animation == false && time_elapsed < death_time) {
    return;
  }

  PLOGI << "The player endured the attack through sheer willpower!";

  health = 1;

  float decrement = morale * 0.50;
  if (decrement > 0) {
    morale -= decrement;
  }

  state = NEUTRAL;
  endure = false;
}

void PlayerCharacter::inputPressed() {
  bool key_right = IsKeyPressed(KEY_RIGHT);
  bool key_left = IsKeyPressed(KEY_LEFT);

  bool key_z = IsKeyPressed(KEY_Z);
  bool key_x = IsKeyPressed(KEY_X);
  bool key_a = IsKeyPressed(KEY_A);
  bool key_s = IsKeyPressed(KEY_S);

  bool key_space = IsKeyPressed(KEY_SPACE);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_right = false;
  bool btn_left = false;

  bool btn_face_right = false;
  bool btn_face_down = false;
  bool btn_face_left = false;
  bool btn_face_up = false;

  bool btn_shoulder_down = false;

  if (gamepad_detected) {
    btn_right = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    btn_left = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);

    btn_face_right = IsGamepadButtonPressed(
      0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT
    );
    btn_face_down = IsGamepadButtonPressed(
      0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN
    );
    btn_face_left = IsGamepadButtonPressed(
      0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT
    );
    btn_face_up = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP);

    btn_shoulder_down = IsGamepadButtonPressed(
      0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) 
      || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2) 
      || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1)
      || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
  }


  bool input_right = key_right || btn_right;
  if (input_right && moving_right == false) {
    moving_right = true;
  }
  
  bool input_left = key_left || btn_left;
  if (input_left && moving_left == false) {
    moving_left = true;
  }

  bool input_light_attack = key_z || btn_face_down;
  if (input_light_attack) {
    input_buffer.push_back(BTN_LIGHT_ATK);
  }

  bool input_heavy_attack = key_x || btn_face_right;
  if (input_heavy_attack) {
    input_buffer.push_back(BTN_HEAVY_ATK);
  }

  bool input_light_tech = key_a || btn_face_left;
  if (input_light_tech) {
    input_buffer.push_back(BTN_LIGHT_TECH);
  }

  bool input_heavy_tech = key_s || btn_face_up;
  if (input_heavy_tech) {
    input_buffer.push_back(BTN_HEAVY_TECH);
  }

  bool input_guard = key_space || btn_shoulder_down;
  if (input_guard) {
    input_buffer.push_back(BTN_GUARD);
  }
}

void PlayerCharacter::inputReleased() {
  bool key_right = IsKeyReleased(KEY_RIGHT);
  bool key_left = IsKeyReleased(KEY_LEFT);

  bool gamepad_detected = IsGamepadAvailable(0);
  bool btn_right = false; 
  bool btn_left = false;

  if (gamepad_detected) {
    btn_right = IsGamepadButtonReleased(
      0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT
    );
    btn_left = IsGamepadButtonReleased(
      0, GAMEPAD_BUTTON_LEFT_FACE_LEFT
    ); 
  }

  bool input_right = key_right || btn_right;
  bool input_left = key_left || btn_left;

  if (input_right && moving_right) {
    moving_right = false;
  }
  if (input_left && moving_left) {
    moving_left = false;
  }
}

void PlayerCharacter::draw(Vector2 &camera_target) {
  if (visible == false) {
    if (DEBUG_MODE) drawDebug();
    return;
  }

  Actor::draw(camera_target); 

  if (CameraUtils::onScreen(this, camera_target) == false) {
    return;
  }

  Rectangle source = {0, 0, tex_scale.x, tex_scale.y};
  if (direction == LEFT) {
    source.width *= -1;
  }

  DrawTexturePro(*current_sprite, source, tex_rect, {0, 0}, 0, WHITE);

  if (DEBUG_MODE == true) {
    drawDebug();
  }
}

void PlayerCharacter::drawDebug() {
  Actor::drawDebug();

  if (isUsingCommand()) {
    current_command->drawDebug();
  }
}
