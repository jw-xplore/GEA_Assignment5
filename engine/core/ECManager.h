#pragma once
#include <vector>
#include <map>
#include <initializer_list>

class Entity;
class ComponentBase;

template <typename T>
class PoolAllocator;

struct EntityId;

class ECManager
{
private:
	unsigned int lastId = 0;

public:
	PoolAllocator<Entity>* entities;

	ECManager();
	~ECManager() {}

	void Update(float dt);
	Entity* AddEntity();
	Entity* AddEntity(std::initializer_list<ComponentBase*> components);
	void RemoveEntity(Entity* entity);
	Entity* FindById(EntityId id);
};