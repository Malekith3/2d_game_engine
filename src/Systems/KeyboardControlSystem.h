//
// Created by sanek on 25/11/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_KEYBOARDCONTROLSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_KEYBOARDCONTROLSYSTEM_H_

#include <SDL.h>
#include "ECS/ECS.h"
#include "Logger/logger.h"
#include "Events/KeyPressedEvent.h"
#include "EventBus/EventBus.h"

class KeyboardControlSystem : public System {
 public:
  KeyboardControlSystem() = default;

  void OnKeyPressed(KeyPressedEvent& event){
    std::string keySymbol = SDL_GetKeyName(event.symbol);
    auto keyCode = std::to_string(event.symbol);
    LOGGER_INFO("[KeyboardControlSystem::OnKeyPressed] Key press event emitted key {} with code {}",keySymbol,keyCode);
  }

  void SubscribeToEvent(std::unique_ptr<EventBus>& eventBus){
    eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
  }

  void Update(){}

};
#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_KEYBOARDCONTROLSYSTEM_H_
