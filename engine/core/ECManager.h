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

	ECManager();
	~ECManager();

	void Update(float dt);
};