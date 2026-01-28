#pragma once
#include <vector>
#include <initializer_list>

struct EntityId
{
	unsigned int id = 0;
};

class Entity
{
private:
	EntityId id;

public:
	//std::vector<ComponentBase*> components;

	Entity(EntityId id, std::initializer_list<unsigned int> comps);
	~Entity();
};