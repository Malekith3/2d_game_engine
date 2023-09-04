//
// Created by Alex Bordeaux on 31/08/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMPONENTS_RIGIDBODYCOMPONENT_H_
#define INC_2D_GAME_ENGINE_SRC_COMPONENTS_RIGIDBODYCOMPONENT_H_
#include "glm/glm.hpp"

struct RigidBodyComponent {
  glm::vec2 m_velocity;

  explicit RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.0,0.0)){
    m_velocity = velocity;
  };

};

#endif //INC_2D_GAME_ENGINE_SRC_COMPONENTS_RIGIDBODYCOMPONENT_H_
