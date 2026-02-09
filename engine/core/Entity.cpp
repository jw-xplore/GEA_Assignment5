#include "config.h"
#include "Entity.h"
#include "ComponentBase.h"

Entity::Entity()
{
	components = std::vector<ComponentBase*>();
}

Entity::Entity(EntityId id): id(id)
{
	components = std::vector<ComponentBase*>();
}

void Entity::Update(float dt)
{
	for (ComponentBase*& comp : components)
	{
		comp->Update(dt);
	}
}

void Entity::SetUnassignedId(unsigned int id)
{
	if (this->id.id != 0)
		return;

	this->id.id = id;
}

void Entity::AddComponent(ComponentBase* component)
{
	components.push_back(component);
	component->Start();
}

void Entity::AddComponents(std::initializer_list<ComponentBase*> components)
{
	for (ComponentBase* comp : components)
	{
		this->components.push_back(comp);
	}

	for (ComponentBase*& comp : this->components)
	{
		comp->Start();
	}
}

void Entity::RemoveComponent(unsigned int componentId)
{
	for (ComponentBase* comp : components)
	{
		if (comp->GetId() == componentId)
		{
			components.erase(std::find(components.begin(), components.end(), comp));
			break;
		}
	}
}