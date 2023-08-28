#include "ECS.h"
#include <algorithm>
#include "Logger/logger.h"

uint32_t Entity::GetId() const {
  return id;
}

bool Entity::operator==(const Entity &rhs) const {
  return id == rhs.id;
}

bool Entity::operator!=(const Entity &rhs) const {
  return !(rhs == *this);
}

template<typename T>
uint32_t Component<T>::GetId() {
  static auto id = nextId++;
  return id;
}

const std::vector<Entity> &System::GetEntities() const {
  return entities;
}

bool System::RemoveEntityFromSystem(Entity &entity) {
  auto find_lambda = [entity](Entity &current_entity){return current_entity == entity;};
  auto result = std::find_if(entities.begin(),entities.end(),find_lambda);
  if(result == entities.end()){
    LOGGER_WARN("[System::RemoveEntityFromSystem] Tried to remove entity that dose not exist, ID: = {}",entity.GetId());
    return false;
  }
  entities.erase(result);
  return true;
}

void System::AddEntityToSystem(Entity &entity) {
  entities.emplace_back(entity);
}
const Signature &System::GetComponentSignature() const {
  return componentSignature;
}

template<typename T>
void System::RequreComponent() {
    const auto componentID = Component<T>::GetId();
    if(componentID > MAX_COMPONENTS){
      LOGGER_WARN("[System::RequreComponent] God Component ID: {} Bigger than max component",componentID);
      return;
    }
  componentSignature.set(componentID);
}
