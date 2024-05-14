// generics.h
/* This file is meant to contain foward declarations for specific base
 * classes and type definitions involving said base classes.*/
#pragma once
#include <vector>
#include <memory>

class Combatant;
class ActionCommand;

typedef std::vector<std::shared_ptr<Combatant>> combatant_list;
