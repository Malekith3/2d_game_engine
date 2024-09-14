//
// Created by Alex Bordeaux on 04/09/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_MOVEMENTSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_MOVEMENTSYSTEM_H_
#include "ECS/ECS.h"
#include "EventBus/EventBus.h"
#include "Events/CollisionEvent.h"

class MovementSystem : public System {
 public:
  MovementSystem();
  ~MovementSystem() = default;
  void Update(double deltaTime);
  void OnCollision(CollisionEvent& event);
  void ChangeDirectionAndFlip(Entity &entity);

  void SubscribeToEvent(std::unique_ptr<EventBus>& eventBus){
    eventBus->SubscribeToEvent<CollisionEvent>(this,&MovementSystem::OnCollision);
  }

};

#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_MOVEMENTSYSTEM_H_
