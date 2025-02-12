// action_command.h
#pragma once
#include <string>
#include <cstdint>
#include <triblib/base/generics.h>

#define CMD_NONE 0
#define CMD_NORM_LIGHT 1
#define CMD_NORM_HEAVY 2
#define CMD_TECH_LIGHT 3
#define CMD_TECH_HEAVY 4


/* The basis for every attack, support, or even utility action in the 
 * game. Typically, the usage of an action command are separated into 3 
 * stages. The charging stage, the action stage, and the recovery stage.
 * 
 * There are 3 methods for handling each stage, and 3 timers for how long
 * each lasts which is defined by whatever class derives from this. The
 * methods' base functionality is to check if enough time has passed to
 * move on to the next stage. They are meant to be overidden and added
 * onto by the derived class.*/
class ActionCommand {
public:
  ActionCommand(Combatant *user, std::string command_name, uint8_t type,
                float charge_time, float act_time, float recovery_time);
  virtual ~ActionCommand() {};

  virtual void chargeSequence(float time_elapsed);

  /* This method is the most special as it executes the main functionality
   * for a given action command. So expect this to be overidden by other
   * classes that derive from this class.*/
  virtual void actSequence(float time_elapsed);

  virtual void recoverySequence(float time_elapsed);

  virtual void draw() {};
  virtual void drawDebug() {};

  std::string command_name;
  uint8_t type;

  float sequence_timestamp;
protected:
  Combatant *user;

  float charge_time;
  bool finished_charge = false;

  float act_time;
  bool finished_action = false;

  float recovery_time;
  bool finished_recovering = false;
};
