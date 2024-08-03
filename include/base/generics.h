// generics.h
/* This file is meant to contain forward declarations for specific
 * classes and type definitions.*/
#pragma once
#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include "dynamic_actor.h"

class Combatant;
class PlayerCharacter;
class ActionCommand;

typedef std::vector<std::shared_ptr<Combatant>> combatant_list;
typedef std::vector<std::unique_ptr<DynamicActor>> dynamic_list;

typedef std::vector<uint8_t> menu_options;
typedef std::vector<std::string> menu_options_txt;

