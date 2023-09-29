//
// Created by sanek on 29/09/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMPONENTS_SPRITECOMPONENT_H_
#define INC_2D_GAME_ENGINE_SRC_COMPONENTS_SPRITECOMPONENT_H_

#include "glm/glm.hpp"

struct SpriteComponent{
  uint32_t m_width;
  uint32_t m_height;

  explicit SpriteComponent(uint32_t width = 1 ,uint32_t height = 1) {
    m_width = width;
    m_height = height;
  }
};


#endif //INC_2D_GAME_ENGINE_SRC_COMPONENTS_SPRITECOMPONENT_H_
