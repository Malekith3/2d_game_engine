//
// Created by sanek on 29/09/2023.
//

#include <SDL.h>
#include <algorithm>
#include "RenderSystem.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"

RenderSystem::RenderSystem() {
  RequreComponent<TransformComponent>();
  RequreComponent<SpriteComponent>();
}

void RenderSystem::Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
  auto allEntities = GetSystemEntities();
  auto sortingLambda = [](Entity& a, Entity& b)
      { return a.GetComponent<SpriteComponent>().m_zIndex < b.GetComponent<SpriteComponent>().m_zIndex;};

  std::sort(allEntities.begin(),allEntities.end(),sortingLambda);
  for(auto entity: allEntities){
    auto& transform = entity.GetComponent<TransformComponent>();
    auto& sprite = entity.GetComponent<SpriteComponent>();
    auto* texture = assetStore->GetTexture(sprite.m_assetId);
    SDL_Rect srcRect = sprite.m_srcRect;
    SDL_Rect dstRect = {
        static_cast<int>(transform.m_position.x),
        static_cast<int>(transform.m_position.y),
        static_cast<int>(transform.m_scale.x * sprite.m_width),
        static_cast<int>(transform.m_scale.y * sprite.m_height)
    };
    SDL_RenderCopyEx(renderer,texture,&srcRect,&dstRect,transform.m_rotation, nullptr, SDL_FLIP_NONE);

  }
}
