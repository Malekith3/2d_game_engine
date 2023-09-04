//
// Created by Alex Bordeaux on 04/09/2023.
//

#include "MovementSystem.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"

MovementSystem::MovementSystem() {
  RequreComponent<TransformComponent>();
  RequreComponent<RigidBodyComponent>();
}

void MovementSystem::Update(double deltaTime) {
    for(auto entity: GetSystemEntities()){
      auto& transform = entity.GetComponent<TransformComponent>();
      auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

      transform.m_position += rigidBody.m_velocity * static_cast<float>(deltaTime);
      LOGGER_WARN("Entity ID = {} moved to x: {} , y: {}",entity.GetId(),transform.m_position.x,transform.m_position.y);
    }
}