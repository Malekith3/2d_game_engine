//
// Created by sanek on 21/11/2023.
//

#include "RenderCollisionSystem.h"
#include "Components/TransformComponent.h"
#include "Components/BoxColliderComponent.h"
#include "SDL.h"
RenderCollisionSystem::RenderCollisionSystem() {
  RequreComponent<TransformComponent>();
  RequreComponent<BoxColliderComponent>();
}

void RenderCollisionSystem::Update(SDL_Renderer *renderer, SDL_Rect &camera) {

    for(auto& entity: GetSystemEntities()){
      auto& transform = entity.GetComponent<TransformComponent>();
      auto& boxCollider = entity.GetComponent<BoxColliderComponent>();

      SDL_Rect colliderRect = {
          static_cast<int>(transform.m_position.x + boxCollider.offset.x - camera.x),
          static_cast<int>(transform.m_position.y + boxCollider.offset.y - camera.y),
          static_cast<int>(boxCollider.width),
          static_cast<int>(boxCollider.height)
      };
      SDL_SetRenderDrawColor(renderer,255,0,0,255);
      SDL_RenderDrawRect(renderer,&colliderRect);
    }
}
