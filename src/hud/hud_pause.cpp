// hud/hud_pause.cpp
#include <raylib.h>
#include "scene_gameplay.h"
#include "hud_menu.h"
#include "hud_pause.h"


PauseHud::PauseHud(GameplayScene *scene) : MenuHud(true) { 
  shift_gradient = false;
  this->scene = scene;
}

void PauseHud::restartOpening() {
  MenuHud::restartOpening();
  fadeInSpade();
}

void PauseHud::checkInput() {

}

void PauseHud::update() {
  MenuHud::update();
}

void PauseHud::draw() {
  MenuHud::draw();
}

