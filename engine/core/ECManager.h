#pragma once
#include <vector>
#include <map>
#include <initializer_list>

class Entity;
class ComponentBase;

class ECManager
{
private:
	unsigned int lastId = 0;

public:
	std::vector<Entity*> entities;

	ECManager();
	~ECManager() {}

	void Update(float dt);
	Entity* AddEntity(std::initializer_list<ComponentBase*> components);
};