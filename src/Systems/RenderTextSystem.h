//
// Created by sanek on 08/06/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERTEXTSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERTEXTSYSTEM_H_

#include "ECS/ECS.h"
#include "Components/TextLabelComponent.h"
#include "AssetStore/AssetStore.h"
class RenderTextSystem : public System
{
 public:
  RenderTextSystem()
  {
    RequreComponent<TextLabelComponent>();
  }

  void Update(std::unique_ptr<AssetStore> &assetStore, SDL_Renderer *renderer, const SDL_Rect &camera)
  {
    for(auto entity : GetSystemEntities())
    {
      const auto textLabel = entity.GetComponent<TextLabelComponent>();

      SDL_Surface* surface = TTF_RenderText_Blended(assetStore->GetFont(textLabel.assetID),
                                                    textLabel.text.c_str(),textLabel.color);

      SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
      SDL_FreeSurface(surface);

      int labelWidth = 0;
      int labelHeight = 0;
      SDL_QueryTexture(texture, nullptr,nullptr, &labelWidth, &labelHeight);

      SDL_Rect dstRect = {static_cast<int>(textLabel.position.x - (textLabel.isFixed ? 0 : camera.x)),
                          static_cast<int>(textLabel.position.y - (textLabel.isFixed ? 0 : camera.y)),
                          labelWidth,
                          labelHeight};

      SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
      SDL_DestroyTexture(texture);
    }
  }
};

#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERTEXTSYSTEM_H_
