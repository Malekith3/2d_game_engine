//
// Created by sanek on 28/01/2024.
//

#include "SDL_rect.h"
#include "CameraMovementSystem.h"
#include "Components/TransformComponent.h"
#include "Components/CameraFollowComponent.h"
#include "Game/Game.h"

CameraMovementSystem::CameraMovementSystem() {
  RequreComponent<CameraFollowComponent>();
}

void CameraMovementSystem::Update(SDL_Rect& camera) {
    for(auto entity: GetSystemEntities()){
      auto transform = entity.GetComponent<TransformComponent>();


      if(Game::mapWidth  > transform.m_position.x + (camera.w / 2)){
        camera.x = transform.m_position.x  - (Game::windowWidth / 2);
      }
      if(Game::mapHeight > transform.m_position.y + (camera.h / 2) ){
        camera.y = transform.m_position.y - (Game::windowHeight / 2);
      }

      camera.x = camera.x < 0 ? 0 : camera.x;
      camera.y = camera.y < 0 ? 0 : camera.y;
      camera.x = camera.x > camera.w ? camera.w : camera.x;
      camera.y = camera.y > camera.h ? camera.h : camera.y;
    }
}
