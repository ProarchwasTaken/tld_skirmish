// effects/fx_dmg_number.h
#include <raylib.h>
#include "globals.h"
#include "base/dynamic_actor.h"
#include "utils.h"
#include "fx_dmg_number.h"
#include <plog/Log.h>


Texture createNumTexture(int value, Vector2 position, Color color) {
  int txt_size = fonts::skirmish->baseSize;

  const char* text = TextFormat("%i", value);
  Image image = ImageTextEx(*fonts::skirmish, text, txt_size, -3, color);

  Texture texture = LoadTextureFromImage(image);
  UnloadImage(image);

  return texture;
}


DamageNumber::DamageNumber(int value, Vector2 position, Color color):
  DynamicActor(position, TYPE_PARTICLE_FX)
{
  number_texture = createNumTexture(value, position, color);

  tex_scale.x = number_texture.width;
  tex_scale.y = number_texture.height;

  tex_offset = {(-tex_scale.x / 2), 0};
  texRectCorrection();

  movement_speed = 0.2;
}

DamageNumber::~DamageNumber() {
  UnloadTexture(number_texture);
}

void DamageNumber::update() {
  movement();
}

void DamageNumber::movement() {
  float magnitude = movement_speed * DELTA_TIME;

  position.y -= magnitude;
  distance_traveled += magnitude;

  if (distance_traveled >= 12) {
    float percentage = 1.0 - ((distance_traveled - 12) / 6);
    tint.a = 255 * percentage;
  }

  if (distance_traveled >= 18) {
    awaiting_deletion = true;
  }

  texRectCorrection();
}

void DamageNumber::draw(Vector2 &camera_target) {
  if (CameraUtils::onScreen(this, camera_target) == false) {
    return;
  }

  Rectangle source = {0, 0, tex_scale.x, tex_scale.y};
  DrawTexturePro(number_texture, source, tex_rect, {0, 0}, 0, tint);
  

  if (DEBUG_MODE == true) {
    drawDebug();
  }
}



