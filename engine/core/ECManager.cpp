#include "config.h"
#include "ECManager.h"
#include "MemoryManager.h"

#include "Entity.h"
#include "ComponentBase.h"

#include "gameplay/TransformCmp.h"
#include "gameplay/RenderableCmp.h"

ECManager::ECManager()
{
	//entities.reserve(256);
	entities = new PoolAllocator<Entity>(256);

	// Setup conmponents
	//transformPool = new PoolAllocator<TransformCmp>(256);
	//renderablePool = new PoolAllocator<RenderableCmp>(256);
}

void ECManager::Update(float dt)
{
	/*
	for (Entity*& entity : entities)
	{
		entity->Update(dt);
	}
	*/

	for (size_t i = 0; i < entities->usedCount; i++)
	{
		entities->at(i).Update(dt);
		//entity->Update(dt);
	}
}

Entity* ECManager::AddEntity(std::initializer_list<ComponentBase*> components)
{
	EntityId id;
	id.id = lastId;
	//Entity* entity = new Entity(id);
	Entity* entity = entities->Allocate();
	entity->components = components;

	size_t size = components.size();
	for (ComponentBase*& comp : entity->components)
	{
		comp->owner = entity;
		comp->Start();
	}

	//entities.push_back(entity);
	return entity;
}