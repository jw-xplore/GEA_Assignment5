#pragma once
#include <vector>
#include <initializer_list>

class ComponentBase;

struct EntityId
{
	unsigned int id = 0;
};

class Entity
{
private:
	EntityId id;

public:
	std::vector<ComponentBase*> components;

	Entity(EntityId id);
	~Entity();

	virtual void Update(float dt) {}

	template<class T>
	T* FindComponent()
	{
		size_t size = components.size();
		for (auto& component : components)
		{
			T* target = dynamic_cast<T*>(component);
			if (target)
				return target;
		}

		return nullptr;
	}
};