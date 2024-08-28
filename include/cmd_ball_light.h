// cmd_ball_light.h
#pragma once
#include <vector>
#include "base/action_command.h"
#include "base/generics.h"
#include "char_player.h"


class BallLight : public ActionCommand {
public:
  BallLight(PlayerCharacter *player);

  void setupHurtbox();
  void chargeSequence(float time_elapsed) override;
  void actSequence(float time_elapsed) override;
private:
  std::vector<int> anim_juke;
  float juke_frametime;

  PlayerCharacter *player;
};
