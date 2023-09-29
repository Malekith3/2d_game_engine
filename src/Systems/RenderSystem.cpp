//
// Created by sanek on 29/09/2023.
//

#include <SDL.h>
#include "RenderSystem.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"

RenderSystem::RenderSystem() {
  RequreComponent<TransformComponent>();
  RequreComponent<SpriteComponent>();
}

void RenderSystem::Update(SDL_Renderer* renderer) {
  for(auto entity: GetSystemEntities()){
    auto& transform = entity.GetComponent<TransformComponent>();
    auto& sprite = entity.GetComponent<SpriteComponent>();
    SDL_Rect object = {static_cast<int>(transform.m_position.x),
                       static_cast<int>(transform.m_position.y),
                       static_cast<int>(sprite.m_width),
                       static_cast<int>(sprite.m_height)
                       };
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer, &object);
  }
}
