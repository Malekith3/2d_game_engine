//
// Created by sanek on 15/09/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_SCRIPTSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_SCRIPTSYSTEM_H_

#include "ECS/ECS.h"
#include "Components/ScriptComponent.h"
#include "Components/TransformComponent.h"

class ScriptSystem : public System
{
 public:

  ScriptSystem()
  {
    RequreComponent<ScriptComponent>();
  }
  void Update(double deltaTime, double elapsedTime);
  void CreateLuaBindings(sol::state& lua);
};

#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_SCRIPTSYSTEM_H_
