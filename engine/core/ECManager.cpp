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

Entity* ECManager::AddEntity(std::initializer_list<ComponentBase*> components)
{
	EntityId id;
	id.id = lastId;
	//Entity* entity = new Entity(id);
	Entity* entity = entities->Allocate();
	entity->components = components;

	// Setup transform
	for (ComponentBase*& comp : entity->components)
	{
		if (comp->GetId() == TransformCmp::CMPID)
		{
			entity->transform = dynamic_cast<TransformCmp*>(comp);
			break;
		}
	}
	
	// Setup all components
	for (ComponentBase*& comp : entity->components)
	{
		comp->owner = entity;
		comp->Start();
	}

	//entities.push_back(entity);
	return entity;
}