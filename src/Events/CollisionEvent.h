//
// Created by sanek on 21/11/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_EVENTS_COLLISIONEVENT_H_
#define INC_2D_GAME_ENGINE_SRC_EVENTS_COLLISIONEVENT_H_
#include "ECS/ECS.h"
#include "EventBus/Event.h"

class CollisionEvent : public Event {
 public:
    Entity firstEntity;
    Entity secondEntity;

  CollisionEvent(Entity& first_entity, Entity& second_entity)
      : firstEntity(first_entity), secondEntity(second_entity) {}
};

#endif //INC_2D_GAME_ENGINE_SRC_EVENTS_COLLISIONEVENT_H_
