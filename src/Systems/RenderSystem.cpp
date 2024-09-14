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

void RenderSystem::Update(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, SDL_Rect &camera) {
  std::vector<Entity> entitiesToRender;
  for(auto& entity : GetSystemEntities())
  {
    auto& transform = entity.GetComponent<TransformComponent>();
    auto& sprite = entity.GetComponent<SpriteComponent>();

    // Check if the entity sprite is outside the camera view
    bool isOutsideCameraView = (
        transform.m_position.x + (transform.m_scale.x * sprite.m_width) < camera.x ||
            transform.m_position.x > camera.x + camera.w ||
            transform.m_position.y + (transform.m_scale.y * sprite.m_height) < camera.y ||
            transform.m_position.y > camera.y + camera.h
    );

    // Cull sprites that are outside the camera view (and are not fixed)
    if (isOutsideCameraView && !sprite.isFixed) {
      continue;
    }

    entitiesToRender.emplace_back(entity);
  }

  auto sortingLambda = [](Entity& a, Entity& b)
      { return a.GetComponent<SpriteComponent>().m_zIndex < b.GetComponent<SpriteComponent>().m_zIndex;};

  std::sort(entitiesToRender.begin(), entitiesToRender.end(), sortingLambda);
  for(auto entity: entitiesToRender){
    auto& transform = entity.GetComponent<TransformComponent>();
    auto& sprite = entity.GetComponent<SpriteComponent>();
    auto* texture = assetStore->GetTexture(sprite.m_assetId);
    SDL_Rect srcRect = sprite.m_srcRect;
    SDL_Rect dstRect = {
        static_cast<int>(transform.m_position.x - (sprite.isFixed ? 0 : camera.x)),
        static_cast<int>(transform.m_position.y - (sprite.isFixed ? 0 : camera.y)),
        static_cast<int>(transform.m_scale.x * sprite.m_width),
        static_cast<int>(transform.m_scale.y * sprite.m_height)
    };
    SDL_RenderCopyEx(renderer,texture,&srcRect,&dstRect,transform.m_rotation, nullptr, sprite._flip);

  }
}
