#include "config.h"
#include "LigthCmp.h"
#include "render/lightserver.h"
#include "TransformCmp.h"

PoolAllocator<LigthCmp> LigthCmp::allocator = PoolAllocator<LigthCmp>(100);

void LigthCmp::Start()
{
	Render::LightServer::CreatePointLight(owner->transform->position, color, intensity, radius);
}