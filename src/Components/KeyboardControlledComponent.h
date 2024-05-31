//
// Created by sanek on 28/01/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMPONENTS_KEYBOARDCONTROLLEDCOMPONENT_H_
#define INC_2D_GAME_ENGINE_SRC_COMPONENTS_KEYBOARDCONTROLLEDCOMPONENT_H_
#include <glm/glm.hpp>

typedef glm::vec2 vec2;
struct KeyboardControlledComponent{
  vec2 upVelocity{};
  vec2 rightVelocity{};
  vec2 leftVelocity{};
  vec2 downVelocity{};

  explicit KeyboardControlledComponent(vec2 upVelocity = vec2(0),vec2 rightVelocity = vec2(0),
                              vec2 leftVelocity = vec2(0),vec2 downVelocity = vec2(0)){
    this->upVelocity    = upVelocity;
    this->rightVelocity = rightVelocity;
    this->leftVelocity  = leftVelocity;
    this->downVelocity  = downVelocity;
  }


};
#endif //INC_2D_GAME_ENGINE_SRC_COMPONENTS_KEYBOARDCONTROLLEDCOMPONENT_H_
