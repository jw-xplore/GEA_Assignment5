#include "config.h"
#include "ECManager.h"

#include "Entity.h"
#include "ComponentBase.h"

ECManager::ECManager()
{
	entities.reserve(256);
}

void ECManager::Update(float dt)
{
	for (Entity*& entity : entities)
	{
		entity->Update(dt);
	}
}

Entity* ECManager::AddEntity(std::initializer_list<ComponentBase*> components)
{
	EntityId id;
	id.id = lastId;
	Entity* entity = new Entity(id);
	entity->components = components;

	size_t size = components.size();
	for (ComponentBase*& comp : entity->components)
	{
		comp->owner = entity;
		comp->Start();
	}

	entities.push_back(entity);
	return entity;
}