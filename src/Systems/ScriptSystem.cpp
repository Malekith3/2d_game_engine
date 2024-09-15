//
// Created by sanek on 15/09/2024.
//

#include "ScriptSystem.h"
#include "Components/RigidBodyComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/ProjectileEmitterComponent.h"

void SetEntityPosition(Entity entity, double x, double y) {
  if (entity.HasComponent<TransformComponent>()) {
    auto& transform = entity.GetComponent<TransformComponent>();
    transform.m_position.x = x;
    transform.m_position.y = y;
  } else {
    LOGGER_ERROR("Trying to set the position of an entity that has no transform component.");
  }
}

std::tuple<double, double> GetEntityPosition(Entity entity) {
  if (entity.HasComponent<TransformComponent>()) {
    const auto transform = entity.GetComponent<TransformComponent>();
    return {transform.m_position.x, transform.m_position.y};
  } else {
    LOGGER_ERROR("Trying to get the position of an entity that has no transform component");
    return {0.0, 0.0};
  }
}

std::tuple<double, double> GetEntityVelocity(Entity entity) {
  if (entity.HasComponent<RigidBodyComponent>()) {
    const auto rigidbody = entity.GetComponent<RigidBodyComponent>();
    return {rigidbody.m_velocity.x, rigidbody.m_velocity.y};
  } else {
    LOGGER_ERROR("Trying to get the velocity of an entity that has no rigidbody component");
    return {0.0, 0.0};
  }
}

void SetEntityVelocity(Entity entity, double x, double y) {
  if (entity.HasComponent<RigidBodyComponent>()) {
    auto& rigidbody = entity.GetComponent<RigidBodyComponent>();
    rigidbody.m_velocity.x = x;
    rigidbody.m_velocity.y = y;
  } else {
    LOGGER_ERROR("Trying to set the velocity of an entity that has no rigidbody component");
  }
}

void SetEntityRotation(Entity entity, double angle) {
  if (entity.HasComponent<TransformComponent>()) {
    auto& transform = entity.GetComponent<TransformComponent>();
    transform.m_rotation = angle;
  } else {
    LOGGER_ERROR("Trying to set the rotation of an entity that has no transform component");
  }
}

void SetEntityAnimationFrame(Entity entity, int frame) {
  if (entity.HasComponent<AnimationComponent>()) {
    auto& animation = entity.GetComponent<AnimationComponent>();
    animation.currentFrame = frame;
  } else {
    LOGGER_ERROR("Trying to set the animation frame of an entity that has no animation component");
  }
}

void SetProjectileVelocity(Entity entity, double x, double y) {
  if (entity.HasComponent<ProjectileEmitterComponent>()) {
    auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
    projectileEmitter.projectileVelocity.x = x;
    projectileEmitter.projectileVelocity.y = y;
  } else {
    LOGGER_ERROR("Trying to set the projectile velocity of an entity that has no projectile emitter component");
  }
}
void ScriptSystem::Update(double deltaTime, double elapsedTime)
{
  for(auto entity: GetSystemEntities())
  {
    const auto scriptComponent = entity.GetComponent<ScriptComponent>();
    scriptComponent._func(entity,deltaTime,elapsedTime);
  }
}

void ScriptSystem::CreateLuaBindings(sol::state &lua)
{
  // Create the "entity" usertype so Lua knows what an entity is
  lua.new_usertype<Entity>(
      "entity",
      "get_id", &Entity::GetId,
      "destroy", &Entity::Kill,
      "has_tag", &Entity::HasTag,
      "belongs_to_group", &Entity::HasGroup
  );

  // Create all the bindings between C++ and Lua functions
  lua.set_function("get_position", GetEntityPosition);
  lua.set_function("get_velocity", GetEntityVelocity);
  lua.set_function("set_position", SetEntityPosition);
  lua.set_function("set_velocity", SetEntityVelocity);
  lua.set_function("set_rotation", SetEntityRotation);
  lua.set_function("set_projectile_velocity", SetProjectileVelocity);
  lua.set_function("set_animation_frame", SetEntityAnimationFrame);
}
