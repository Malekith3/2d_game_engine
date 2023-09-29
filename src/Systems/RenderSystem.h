//
// Created by sanek on 29/09/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERSYSTEM_H_

#include "ECS/ECS.h"

class RenderSystem : public System {
 public:
  RenderSystem();
  ~RenderSystem() = default;
  void Update(SDL_Renderer* renderer);

};


#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERSYSTEM_H_
