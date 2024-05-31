//
// Created by sanek on 31/05/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_PROJECTILELIFECYCLESYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_PROJECTILELIFECYCLESYSTEM_H_
#include "ECS/ECS.h"
#include "Components/ProjectileComponent.h"
class ProjectileLifecycleSystem: public System
{
 public:
  ProjectileLifecycleSystem()
  {
    RequreComponent<ProjectileComponent>();
  }

  void Update()
  {
    for (auto entity : GetSystemEntities()) {
        auto& projectile = entity.GetComponent<ProjectileComponent>();

        if(SDL_GetTicks() - projectile.startTime > projectile.duration)
        {
          entity.Kill();
        }
    }
  }
};
#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_PROJECTILELIFECYCLESYSTEM_H_
