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

void Entity::Tag(std::string_view tag)
{
  registry->TagEntity(*this, tag);
}

bool Entity::HasTag(std::string_view tag) const
{
  return registry->EntityHasTag(*this, tag);
}

void Entity::Group(std::string_view group)
{
  registry->GroupEntity(*this, group);
}

bool Entity::HasGroup(std::string_view group) const
{
  return registry->EntityBelongsToGroup(*this, group);
}

const std::vector<Entity> &System::GetEntities() const
{
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

    //Remove the entity from the component pools
    for(auto pool: componentPools)
    {
      if(pool)
      {
        pool->RemoveEntityFromPool(entity.GetId());
      }
    }

    freeIds.push_back(entity.GetId());

    RemoveEntityTag(entity);
    RemoveEntityGroup(entity);
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

void Registry::TagEntity(Entity entity, std::string_view tag)
{
  entityPerTag.emplace(std::make_pair(tag,entity));
  tagPerEntity.emplace(std::make_pair(entity.GetId(),tag));
}

bool Registry::EntityHasTag(Entity entity, std::string_view tag) const
{
  auto iterator = tagPerEntity.find(entity.GetId());
  return iterator != tagPerEntity.end() && iterator->second == tag;
}

Entity Registry::GetEntityByTag(std::string_view tag) const
{
  return entityPerTag.at(std::string(tag));
}

void Registry::RemoveEntityTag(Entity entity)
{
  if(tagPerEntity.find(entity.GetId()) == tagPerEntity.end())
  {
    return;
  }

  auto tag = tagPerEntity[entity.GetId()];
  entityPerTag.erase(tag);
  tagPerEntity.erase(entity.GetId());
}

void Registry::GroupEntity(Entity entity, std::string_view group)
{
  entityPerGroup[std::string(group)].emplace(entity.GetId());
  groupPerEntity.emplace(entity.GetId(),group);
}

bool Registry::EntityBelongsToGroup(Entity entity, std::string_view group) const
{
  auto groupName = groupPerEntity.find(entity.GetId());
  return groupName != groupPerEntity.end() && groupName->second == group;
}

std::vector<Entity> Registry::GetEntitiesByGroup(std::string_view group) const
{
  if(entityPerGroup.find(std::string(group)) == entityPerGroup.end())
    return {};

  auto entitiesInGroup = entityPerGroup.at(std::string(group));
  return {entitiesInGroup.begin(),entitiesInGroup.end()};
}

void Registry::RemoveEntityGroup(Entity entity)
{
  if(groupPerEntity.find(entity.GetId()) == groupPerEntity.end())
  {
    return;
  }

  auto tag = groupPerEntity[entity.GetId()];

  if(entityPerGroup[tag].find(entity) == entityPerGroup[tag].end())
  {
    return;
  }

  entityPerGroup[tag].erase(entity);
  groupPerEntity.erase(entity.GetId());

}





