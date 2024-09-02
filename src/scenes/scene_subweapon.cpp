// scenes/scene_subweapon.cpp
#include <functional>
#include "base/scene.h"
#include "scene_subweapon.h"

using std::function;


SubWeaponScene::SubWeaponScene(function<void(int)> load_scene):
  Scene(load_scene)
{

}

void SubWeaponScene::drawScene() {

}
