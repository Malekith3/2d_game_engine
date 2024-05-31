//
// Created by sanek on 28/01/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_CAMERAMOVEMENTSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_CAMERAMOVEMENTSYSTEM_H_
#include "ECS/ECS.h"


class CameraMovementSystem : public System {
 public:
  CameraMovementSystem();

  void Update(SDL_Rect& camera);
};

#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_CAMERAMOVEMENTSYSTEM_H_
