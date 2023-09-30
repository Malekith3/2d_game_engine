//
// Created by sanek on 29/09/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMPONENTS_SPRITECOMPONENT_H_
#define INC_2D_GAME_ENGINE_SRC_COMPONENTS_SPRITECOMPONENT_H_

#include "SDL.h"
#include <string>
#include <utility>
#include "glm/glm.hpp"

struct SpriteComponent{
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_zIndex;
  std::string m_assetId;
  SDL_Rect m_srcRect{};

  explicit SpriteComponent(uint32_t width = 1 ,uint32_t height = 1,std::string assetId = "",
                            int zIndex = 0,int srcRectX = 0,int srcRectY = 0) {
    m_width = width;
    m_height = height;
    m_zIndex = zIndex;
    m_assetId = std::move(assetId);
    m_srcRect = {srcRectX,srcRectY, static_cast<int>(width), static_cast<int>(width)};
  }
};


#endif //INC_2D_GAME_ENGINE_SRC_COMPONENTS_SPRITECOMPONENT_H_
