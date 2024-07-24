// generics.h
/* This file is meant to contain forward declarations for specific base
 * classes and type definitions.*/
#pragma once
#include <vector>
#include <memory>
#include <string>
#include <cstdint>

class Combatant;
class PlayerCharacter;
class ActionCommand;

typedef std::vector<std::shared_ptr<Combatant>> combatant_list;

typedef std::vector<uint8_t> menu_options;
typedef std::vector<std::string> menu_options_txt;

