//
// Created by sanek on 21/11/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_COLLISIONSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_COLLISIONSYSTEM_H_
#include "ECS/ECS.h"
#include "EventBus/EventBus.h"
class CollisionSystem : public System {
 public:
  CollisionSystem();
  void CollisionSystem::Update(std::unique_ptr<EventBus>& eventBus);
};

struct CollisionBoxWrapper{
  double posX;
  double posY;
  double width;
  double height;

  CollisionBoxWrapper::CollisionBoxWrapper(double pos_x, double pos_y, double width, double height)
      : posX(pos_x), posY(pos_y), width(width), height(height) {}
};

bool CheckAABBCollision(CollisionBoxWrapper& boxColliderA,CollisionBoxWrapper boxColliderB);
#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_COLLISIONSYSTEM_H_
