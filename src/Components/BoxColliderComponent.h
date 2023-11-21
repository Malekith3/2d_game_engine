//
// Created by sanek on 21/11/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMPONENTS_BOXCOLLIDERCOMPONENT_H_
#define INC_2D_GAME_ENGINE_SRC_COMPONENTS_BOXCOLLIDERCOMPONENT_H_
#include "glm/vec2.hpp"
struct BoxColliderComponent{
  int height;
  int width;
  glm::vec2 offset;

  BoxColliderComponent(int height=1, int width=1, const glm::vec2 &offset = glm::vec2(0)) :
  height(height), width(width), offset(offset) {}
};
#endif //INC_2D_GAME_ENGINE_SRC_COMPONENTS_BOXCOLLIDERCOMPONENT_H_
