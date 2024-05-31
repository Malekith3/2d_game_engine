//
// Created by sanek on 03/02/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_PROJECTILEEMITSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_PROJECTILEEMITSYSTEM_H_
#include <ECS/ECS.h>
#include <SDL2/SDL.h>
#include "glm/vec2.hpp"
#include "Components/ProjectileEmitterComponent.h"
#include "Events/KeyPressedEvent.h"
#include "EventBus/EventBus.h"

class ProjectileEmitSystem : public System {
 public:
  ProjectileEmitSystem();
  ~ProjectileEmitSystem() override = default;

  void Update(std::unique_ptr<Registry>& registry);
  void OnKeyPressed(KeyPressedEvent& event);
  void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
  {
    eventBus->SubscribeToEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::OnKeyPressed);
  }

 private:
  void InitProjectile(Entity &projectile, glm::vec2 &projectilePosition, glm::vec2 &projectileVelocity, ProjectileEmitterComponent &emitter);
};

#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_PROJECTILEEMITSYSTEM_H_
