//
// Created by sanek on 21/11/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERCOLLISIONSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERCOLLISIONSYSTEM_H_
#include "ECS/ECS.h"
#include "SDL_render.h"

class RenderCollisionSystem : public System {
 public:
  RenderCollisionSystem();
  void Update(SDL_Renderer *renderer);
};

#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERCOLLISIONSYSTEM_H_
