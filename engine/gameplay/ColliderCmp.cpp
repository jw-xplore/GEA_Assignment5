#include "config.h"
#include "ColliderCmp.h"
#include "TransformCmp.h"
#include "render/physics.h"

PoolAllocator<ColliderCmp> ColliderCmp::allocator = PoolAllocator<ColliderCmp>(200);

void ColliderCmp::Start()
{
    transform = owner->transform;
}

void ColliderCmp::Update(float dt)
{
    Physics::SetTransform(*colId, transform->transform);
}