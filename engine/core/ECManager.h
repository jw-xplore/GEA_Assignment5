#pragma once
#include <vector>
#include <map>

class Entity;
class ComponentBase;

class ECManager
{
private:
	unsigned int lastId = 0;

public:
	std::vector<Entity> entities;
	std::vector<std::vector<ComponentBase>> components;

	

	ECManager();
	~ECManager();

	void AddEntity(std::initializer_list<unsigned int> comps);
};