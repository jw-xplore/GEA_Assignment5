#include "config.h"
#include "ECManager.h"

#include "Entity.h"
#include "ComponentBase.h"

ECManager::ECManager()
{

}

void ECManager::Update(float dt)
{
	for (Entity& entity : entities)
	{
		entity.Update(dt);
	}
}

