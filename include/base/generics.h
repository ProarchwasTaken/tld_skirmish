// generics.h
#pragma once
#include <vector>
#include <memory>

class Combatant;
class ActionCommand;

typedef std::vector<std::shared_ptr<Combatant>> combatant_list;
