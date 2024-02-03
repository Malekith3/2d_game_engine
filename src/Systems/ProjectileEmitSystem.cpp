//
// Created by sanek on 03/02/2024.
//

#include "ProjectileEmitSystem.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/ProjectileEmitterComponent.h"

ProjectileEmitSystem::ProjectileEmitSystem() {
  RequreComponent<TransformComponent>();
  RequreComponent<ProjectileEmitterComponent>();
}

void ProjectileEmitSystem::Update(std::unique_ptr<Registry> &registry) {
    for(auto entity: GetSystemEntities()){
      auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
      const auto transform = entity.GetComponent<TransformComponent>();

      if(SDL_GetTicks() - projectileEmitter.lastEmissionTime > projectileEmitter.repeatedFrequency){
        glm::vec2  projectilePosition = transform.m_position;
        if(entity.HasComponent<SpriteComponent>()){
          const auto sprite = entity.GetComponent<SpriteComponent>();
          projectilePosition.x += ( transform.m_scale.x * sprite.m_width / 2 );
          projectilePosition.y += ( transform.m_scale.y * sprite.m_height / 2 );
        }
        auto projectile = registry->CreateEntity();
        InitProjectile(projectile,projectilePosition,projectileEmitter.projectileVelocity);
        projectileEmitter.lastEmissionTime = SDL_GetTicks();
      }
    }
}

void ProjectileEmitSystem::InitProjectile(Entity &projectile,glm::vec2 &projectilePosition, 
                                          glm::vec2 &projectileVelocity){
  const static glm::vec2 kDefaultScale = {1.0, 1.0};
  const static float kDefaultRotation = 0.0;
  projectile.AddComponent<TransformComponent>(projectilePosition,kDefaultScale, kDefaultRotation);
  projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
  projectile.AddComponent<SpriteComponent>(4,4,"bullet-image",4);
  projectile.AddComponent<BoxColliderComponent>(4,4);
}



