//
// Created by Alex Bordeaux on 04/09/2023.
//

#include "MovementSystem.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Game/Game.h"
#include "Common/TagGroups.h"
#include "Components/SpriteComponent.h"

constexpr int PADDING_LEFT = 10;
constexpr int PADDING_TOP = 10;
constexpr int PADDING_RIGHT = 50;
constexpr int PADDING_BOTTOM = 50;


MovementSystem::MovementSystem() {
  RequreComponent<TransformComponent>();
  RequreComponent<RigidBodyComponent>();
}

void MovementSystem::Update(double deltaTime) {
    for(auto entity: GetSystemEntities()){
      auto& transform = entity.GetComponent<TransformComponent>();
      auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

      transform.m_position += rigidBody.m_velocity * static_cast<float>(deltaTime);

      bool isEntityOutsideOfMap = (
          transform.m_position.x < 0 ||
          transform.m_position.x > Game::mapWidth ||
          transform.m_position.y < 0 ||
          transform.m_position.y > Game::mapHeight
          );

      if (entity.HasTag("player"))
      {
        transform.m_position.x = transform.m_position.x < PADDING_LEFT ? PADDING_LEFT : transform.m_position.x;
        transform.m_position.x = transform.m_position.x > Game::mapWidth - PADDING_RIGHT ? Game::mapWidth - PADDING_RIGHT : transform.m_position.x;
        transform.m_position.y = transform.m_position.y < PADDING_TOP ? PADDING_TOP : transform.m_position.y;
        transform.m_position.y = transform.m_position.y > Game::mapHeight - PADDING_BOTTOM ? Game::mapHeight - PADDING_BOTTOM : transform.m_position.y;
      }

      if(isEntityOutsideOfMap && !entity.HasTag(getTagNameString(TAG_GROUPS::PLAYER)))
      {
        entity.Kill();
      }
    }
}
void MovementSystem::OnCollision(CollisionEvent &event)
{
  Entity a = event.firstEntity;
  Entity b = event.secondEntity;

  if(a.HasGroup(getTagNameString(TAG_GROUPS::OBSTACLES)) &&
     b.HasGroup(getTagNameString(TAG_GROUPS::ENEMIES)))
  {
    ChangeDirectionAndFlip(b);
  }

  if(b.HasGroup(getTagNameString(TAG_GROUPS::OBSTACLES)) &&
      a.HasGroup(getTagNameString(TAG_GROUPS::ENEMIES)))
  {
    ChangeDirectionAndFlip(a);
  }

}

void MovementSystem::ChangeDirectionAndFlip(Entity &entity)
{
  if(!entity.HasComponent<RigidBodyComponent>() && !entity.HasComponent<SpriteComponent>())
  {
    LOGGER_DEBUG("[MovementSystem::ChangeDirectionAndFlip] Collision event on entity without Rigid Body or sprite component");
  }

  auto& rigidBodyComp = entity.GetComponent<RigidBodyComponent>();
  auto& spriteComponent = entity.GetComponent<SpriteComponent>();

  if(rigidBodyComp.m_velocity.x != 0)
  {
    rigidBodyComp.m_velocity.x *= -1;
    spriteComponent._flip = (spriteComponent._flip == SDL_FLIP_NONE) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
  }

  if(rigidBodyComp.m_velocity.y != 0)
  {
    rigidBodyComp.m_velocity.y *= -1;
    spriteComponent._flip = (spriteComponent._flip == SDL_FLIP_NONE) ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
  }

  // TODO need to add flip when there is x and y mov directions
}
