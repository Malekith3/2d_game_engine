#ifndef ECS_H
#define ECS_H

#include <cstdint>
#include <bitset>
#include <vector>


const uint32_t  MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

struct BComponent{
 protected:
  static  uint32_t  nextId;
};

template<typename T>
class Component : BComponent{
 private:
  uint32_t id;
 public:
  static uint32_t GetId();
};


class Entity{
 private:
  uint32_t id;
  explicit Entity(uint32_t id) : id(id){};
 public:
  uint32_t GetId() const;
  bool operator==(const Entity &rhs) const;
  bool operator!=(const Entity &rhs) const;
};

class System{
 private:
  Signature componentSignature;
  std::vector<Entity> entities;
 public:
  System()  = default;
  virtual ~System() = default;
  void AddEntityToSystem(Entity &entity);
  bool RemoveEntityFromSystem(Entity &entity);
  const std::vector<Entity> &GetEntities() const;
  const Signature &GetComponentSignature() const;

  template<typename TComponent> void RequreComponent();
};

class Registry{

};

#endif