#include "config.h"
#include "ECManager.h"
#include "MemoryManager.h"

#include "Entity.h"
#include "ComponentBase.h"

#include "gameplay/TransformCmp.h"
#include "gameplay/RenderableCmp.h"

#include <iostream>

ECManager::ECManager()
{
	entities = new PoolAllocator<Entity>(256);
}

void ECManager::Update(float dt)
{
	for (size_t i = 0; i < entities->usedCount; i++)
	{
		entities->at(i).Update(dt);
	}
}

Entity* ECManager::AddEntity()
{
	Entity* entity = entities->Allocate();
	entity->SetUnassignedId(lastId);
	lastId++;

	return entity;
}

Entity* ECManager::AddEntity(std::initializer_list<ComponentBase*> components)
{
	Entity* entity = entities->Allocate();
	entity->SetUnassignedId(lastId);
	lastId++;
	entity->components = components;
	
	size_t compSize = entity->components.size();

	// Assign and start components
	for (size_t i = 0; i < compSize; i++)
	{
		ComponentBase*& comp = entity->components[i];

		// Setup transform
		if (comp->GetId() == TransformCmp::CMPID)
		{
			entity->transform = dynamic_cast<TransformCmp*>(comp);
		}

		comp->owner = entity;
	}
	
	for (size_t i = 0; i < compSize; i++)
	{
		ComponentBase*& comp = entity->components[i];

		comp->Start();
	}

	return entity;
}