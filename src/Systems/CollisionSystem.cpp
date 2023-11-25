//
// Created by sanek on 21/11/2023.
//

#include "CollisionSystem.h"
#include "Components/TransformComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Logger/logger.h"
#include "Events/CollisionEvent.h"

CollisionSystem::CollisionSystem() {
  RequreComponent<TransformComponent>();
  RequreComponent<BoxColliderComponent>();
}

void CollisionSystem::Update(std::unique_ptr<EventBus>& eventBus) {
    auto& entities = GetSystemEntities();

    for (auto i = entities.begin(); i != entities.end();++i){
      Entity entityA = *i;

      auto& aTransform = entityA.GetComponent<TransformComponent>();
      auto& aCollider = entityA.GetComponent<BoxColliderComponent>();

      for(auto j = i + 1;j!= entities.end();++j){
        auto entityB = *j;

        auto& bTransform = entityB.GetComponent<TransformComponent>();
        auto& bCollider = entityB.GetComponent<BoxColliderComponent>();
        CollisionBoxWrapper boxColliderA(aTransform.m_position.x + aCollider.offset.x,
                                         aTransform.m_position.y + aCollider.offset.y,
                                         aCollider.width,aCollider.height);
        CollisionBoxWrapper boxColliderB(bTransform.m_position.x + bCollider.offset.x,
                                         bTransform.m_position.y + bCollider.offset.y,
                                         bCollider.width,bCollider.height);
        if(CheckAABBCollision(boxColliderA,boxColliderB)){
          LOGGER_ERROR("[COLLISION DETECTION] Entity {} collided with Entity {}", entityA.GetId(),entityB.GetId());
          eventBus->EmitEvent<CollisionEvent>(entityA,entityB);
        }
      }
    }
}


bool CheckAABBCollision(CollisionBoxWrapper& boxColliderA,CollisionBoxWrapper boxColliderB){
  return (
      boxColliderA.posX < boxColliderB.posX + boxColliderB.width &&
      boxColliderA.posX + boxColliderA.width > boxColliderB.posX &&
      boxColliderA.posY < boxColliderB.posY + boxColliderB.height &&
      boxColliderA.posY + boxColliderA.height > boxColliderB.posY
      );
}