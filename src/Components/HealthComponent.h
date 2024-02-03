//
// Created by sanek on 03/02/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMPONENTS_HEALTHCOMPONENT_H_
#define INC_2D_GAME_ENGINE_SRC_COMPONENTS_HEALTHCOMPONENT_H_
struct HealthComponent{
  unsigned int health;

  explicit HealthComponent(unsigned int health = 0) : health(health) {}
};
#endif //INC_2D_GAME_ENGINE_SRC_COMPONENTS_HEALTHCOMPONENT_H_
