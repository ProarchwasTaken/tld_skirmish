// cmd_ball_light.h
#pragma once
#include <vector>
#include "base/action_command.h"
#include "base/generics.h"
#include "char_player.h"


/* Light weapon technique that's associated with the Ball Sub-Weapon. 
 * The player automatically runs forward at a fast pace while dribbling
 * the ball. Basically performing a "juking" manuver of sorts. During the 
 * active phase, the player is invulnerable, and is able to change 
 * direction. They are also able to cancel the action command to any 
 * other commands that isn't of the "LIGHT" type.*/
class BallLight : public ActionCommand {
public:
  BallLight(PlayerCharacter *player);
  ~BallLight();

  void setupHurtbox();
  void chargeSequence(float time_elapsed) override;
  void actSequence(float time_elapsed) override;
private:
  std::vector<int> anim_juke;
  float juke_frametime;

  PlayerCharacter *player;
};
