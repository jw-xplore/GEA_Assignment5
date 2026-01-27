#pragma once
#include <vector>
#include <initializer_list>

class Entity
{
private:
	unsigned int id = -1;
	int a

public:
	std::vector<unsigned int> components;

	Entity(unsigned int id, std::initializer_list<unsigned int> comps);
	~Entity();
};