//
// Created by sanek on 25/11/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_KEYBOARDCONTROLSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_KEYBOARDCONTROLSYSTEM_H_

#include <SDL.h>
#include "ECS/ECS.h"
#include "Logger/logger.h"
#include "EventBus/EventBus.h"
#include "Events/KeyPressedEvent.h"
#include "Components/KeyboardControlledComponent.h"

class KeyboardControlSystem : public System {
 public:
  KeyboardControlSystem(){
    RequreComponent<KeyboardControlledComponent>();
    RequreComponent<SpriteComponent>();
    RequreComponent<RigidBodyComponent>();
  };

  void OnKeyPressed(KeyPressedEvent& event){
    for(auto entity:GetSystemEntities()){
      auto keyboardControl = entity.GetComponent<KeyboardControlledComponent>();
      auto& sprite = entity.GetComponent<SpriteComponent>();
      auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

      switch (event.symbol) {
        case SDLK_UP:
          rigidBody.m_velocity = keyboardControl.upVelocity;
          sprite.m_srcRect.y = sprite.m_height * SPRITE_ROW::ZERO;
          break;
        case SDLK_RIGHT:
          rigidBody.m_velocity = keyboardControl.rightVelocity;
          sprite.m_srcRect.y = sprite.m_height * SPRITE_ROW::ONE;
          break;
        case SDLK_LEFT:
          rigidBody.m_velocity = keyboardControl.leftVelocity;
          sprite.m_srcRect.y = sprite.m_height * SPRITE_ROW::THREE  ;
          break;
        case SDLK_DOWN:
          rigidBody.m_velocity = keyboardControl.downVelocity;
          sprite.m_srcRect.y = sprite.m_height * SPRITE_ROW::TWO;
          break;

        default:
          break;
      }
      
    }
  }

  void SubscribeToEvent(std::unique_ptr<EventBus>& eventBus){
    eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
  }

  void Update(){}

 private:
  enum SPRITE_ROW{ZERO,ONE,TWO,THREE};

};
#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_KEYBOARDCONTROLSYSTEM_H_
