//
// Created by sanek on 03/02/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_PROJECTILEEMITSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_PROJECTILEEMITSYSTEM_H_
#include <ECS/ECS.h>
#include <SDL2/SDL.h>
#include "glm/vec2.hpp"

class ProjectileEmitSystem : public System {
 public:
  ProjectileEmitSystem();
  void Update(std::unique_ptr<Registry>& registry);
  ~ProjectileEmitSystem() override = default;

 private:
  void InitProjectile(Entity& projectile, glm::vec2& projectilePosition, glm::vec2& projectileVelocity);
};

#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_PROJECTILEEMITSYSTEM_H_
