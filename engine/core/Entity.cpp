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