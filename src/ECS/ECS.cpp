#include "ECS.h"
#include <algorithm>

uint32_t Entity::GetId() const {
  return id;
}

bool Entity::operator==(const Entity &rhs) const {
  return id == rhs.id;
}

bool Entity::operator!=(const Entity &rhs) const {
  return !(rhs == *this);
}
bool Entity::operator<(const Entity &other) const {
  return id < other.id;
}
void Entity::Kill() {
  registry->KillEntity(*this);
}

const std::vector<Entity> &System::GetEntities() const {
  return entities;
}

bool System::RemoveEntityFromSystem(Entity &entity) {
  auto find_lambda = [entity](Entity &current_entity){return current_entity == entity;};
  auto result = std::remove_if(entities.begin(),entities.end(),find_lambda);
  if(result == entities.end()){
    LOGGER_WARN("[System::RemoveEntityFromSystem] Tried to remove entity that dose not exist, ID: = {}",entity.GetId());
    return false;
  }
  entities.erase(result,entities.end());
  return true;
}

void System::AddEntityToSystem(const Entity &entity) {
  entities.emplace_back(entity);
}
const Signature &System::GetComponentSignature() const {
  return componentSignature;
}
const std::vector<Entity> &System::GetSystemEntities() const {
  return entities;
}

Entity Registry::CreateEntity() {
  int entityId;
  if(freeIds.empty()){
    entityId = numEntities++;
    if(entityId >= entityComponentSignatures.size()){
      entityComponentSignatures.resize(entityId+1);
    }
  }
  else{
    entityId = freeIds.front();
    freeIds.pop_front();
  }

  Entity entity(entityId);
  entity.registry = this;

  entitiesToBeAdded.emplace(entity);
  LOGGER_TRACE("[Registry::CreateEntity] Entity created with ID = {}", entityId);
  return entity;
}

void Registry::Update() {
  for(auto& entity:entitiesToBeAdded){
    AddEntityToSystem(entity);
  }
  entitiesToBeAdded.clear();

  for(auto entity: entitiesToBeKilled){
    RemoveEntityFromSystem(entity);
    entityComponentSignatures[entity.GetId()].reset();

    freeIds.push_back(entity.GetId());
  }
  entitiesToBeKilled.clear();

}

void Registry::AddEntityToSystem(const Entity &entity) {
  const auto entityId = entity.GetId();

  const auto& entityComponentSignature = entityComponentSignatures[entityId];

  for (auto& system: systems) {
    const auto& systemComponentSignature = system.second->GetComponentSignature();

    bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

    if (isInterested) {
      system.second->AddEntityToSystem(entity);
    }
  }
}

void Registry::KillEntity(Entity &entity) {
    entitiesToBeKilled.emplace(entity);
}

void Registry::RemoveEntityFromSystem(Entity &entity) {
  for(auto& system : systems){
      system.second->RemoveEntityFromSystem(entity);
  }
}





