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

      void OnCollision(CollisionEvent& event){
        LOGGER_WARN("[DamageSystem::OnCollision] damage Inflicted on Entity {} and Entity {}",
                    event.firstEntity.GetId(),event.secondEntity.GetId());
        //event.firstEntity.Kill();
        //event.secondEntity.Kill();
      }
};

#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_DAMAGESYSTEM_H_
