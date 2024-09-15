//
// Created by sanek on 15/09/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMPONENTS_SCRIPTCOMPONENT_H_
#define INC_2D_GAME_ENGINE_SRC_COMPONENTS_SCRIPTCOMPONENT_H_
#include "sol/sol.hpp"
struct ScriptComponent
{
 sol::function _func;

  explicit ScriptComponent(const sol::function func = sol::nil) : _func(func) {}
};
#endif //INC_2D_GAME_ENGINE_SRC_COMPONENTS_SCRIPTCOMPONENT_H_
