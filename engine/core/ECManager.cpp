#include "ECManager.h"
//#include "Entity.h"
//#include "ComponentBase.h"

ECManager::ECManager()
{
	/*
	components.reserve(EComponents::EComponentsCount);

	for (size_t i = 0; i < EComponents::EComponentsCount; i++)
	{
		std::vector<ComponentBase> comps;
		comps.reserve(64);

		components.push_back(comps);
	}
	*/
}

void ECManager::AddEntity(std::initializer_list<unsigned int> comps)
{
	/*
	entities.push_back(Entity(EntityId{ lastId }, comps));
	for (auto& compId : comps)
	{
		//components[compId].push_back(blueprints[compId]);
	}
	*/

	lastId++;
}