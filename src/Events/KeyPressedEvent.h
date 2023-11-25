//
// Created by sanek on 25/11/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_EVENTBUS_KEYPRESSEDEVENT_H_
#define INC_2D_GAME_ENGINE_SRC_EVENTBUS_KEYPRESSEDEVENT_H_

#include "SDL2/SDL.h"
#include "EventBus/Event.h"
#include "ECS/ECS.h"

class KeyPressedEvent: public Event {
 public:
  SDL_KeyCode  symbol;
  explicit KeyPressedEvent(SDL_KeyCode symbol) : symbol(symbol) {}
};

#endif //INC_2D_GAME_ENGINE_SRC_EVENTBUS_KEYPRESSEDEVENT_H_
