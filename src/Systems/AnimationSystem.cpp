//
// Created by sanek on 19/11/2023.
//

#include "AnimationSystem.h"
#include "Components/SpriteComponent.h"
#include "Components/AnimationComponent.h"
constexpr int kMiliInSec = 1000;
AnimationSystem::AnimationSystem() {
  RequreComponent<SpriteComponent>();
  RequreComponent<AnimationComponent>();
}

void AnimationSystem::Update() {
  for(auto& entity: GetSystemEntities()){
    auto& animation = entity.GetComponent<AnimationComponent>();
    auto& sprite = entity.GetComponent<SpriteComponent>();

    animation.currentFrame = ((SDL_GetTicks() - animation.startTime)
        * animation.frameSpeedRate / kMiliInSec) % animation.numFrames;
    sprite.m_srcRect.x = animation.currentFrame * sprite.m_width;
  }
}
