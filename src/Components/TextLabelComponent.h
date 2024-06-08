//
// Created by sanek on 08/06/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMPONENTS_TEXTLABELCOMPONENT_H_
#define INC_2D_GAME_ENGINE_SRC_COMPONENTS_TEXTLABELCOMPONENT_H_

#include <string>
#include <utility>
#include "glm/vec2.hpp"
#include "SDL.h"

struct TextLabelComponent
{
  glm::vec2 position;
  std::string text;
  std::string  assetID;
  SDL_Color color;
  bool isFixed;

  TextLabelComponent(const glm::vec2 &position = {0,0},
                     std::string text = "",
                     std::string asset_id = "",
                     const SDL_Color &color = {255,255,255,1},
                     bool is_fixed = true)
      : position(position), text(std::move(text)), assetID(std::move(asset_id)), color(color), isFixed(is_fixed) {}
};
#endif //INC_2D_GAME_ENGINE_SRC_COMPONENTS_TEXTLABELCOMPONENT_H_
