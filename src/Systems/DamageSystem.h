//
// Created by sanek on 25/11/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_DAMAGESYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_DAMAGESYSTEM_H_
#include "ECS/ECS.h"
#include "Logger/logger.h"
#include "EventBus/EventBus.h"
#include "Events/CollisionEvent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/ProjectileComponent.h"
#include "Components/HealthComponent.h"

class DamageSystem : public System{
 public:
      DamageSystem(){
        RequreComponent<BoxColliderComponent>();
      }

      void SubscribeToEvent(std::unique_ptr<EventBus>& eventBus){
        eventBus->SubscribeToEvent<CollisionEvent>(this,&DamageSystem::OnCollision);
      }

      void Update(){

      }

      void OnCollision(CollisionEvent& event)
      {
        Entity a = event.firstEntity;
        Entity b = event.secondEntity;
        LOGGER_DEBUG("[DamageSystem::OnCollision] damage Inflicted on Entity {} and Entity {}",
                    event.firstEntity.GetId(),event.secondEntity.GetId());

        if(a.HasGroup("projectiles") && b.HasTag("player"))
        {
          OnProjectileHitsPlayer(a,b);
        }

        if(b.HasGroup("projectiles") && a.HasTag("player"))
        {
          OnProjectileHitsPlayer(b,a);
        }

        if(a.HasGroup("projectiles") && b.HasGroup("enemies"))
        {
          OnProjectileHitsEnemy(a,b);
        }

        if(b.HasGroup("projectiles") && a.HasGroup("enemies"))
        {
          OnProjectileHitsEnemy(b,a);
        }

      }

 private:
      void OnProjectileHitsPlayer(Entity projectile, Entity player)
      {
        auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if(projectileComponent.isFriendly)
          return;

        auto& playerHealth = player.GetComponent<HealthComponent>();
        playerHealth.health -= projectileComponent.hitPercentileDamage;

        projectile.Kill();

        if(playerHealth.health <= 0)
          player.Kill();
      }

      void OnProjectileHitsEnemy(Entity projectile, Entity enemy)
      {
        auto projectileComponent = projectile.GetComponent<ProjectileComponent>();

        if (!projectileComponent.isFriendly)
          return;

        auto &enemyHealth = enemy.GetComponent<HealthComponent>();
        enemyHealth.health -= projectileComponent.hitPercentileDamage;

        projectile.Kill();

        if (enemyHealth.health <= 0)
          enemy.Kill();
      }
};

#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_DAMAGESYSTEM_H_
