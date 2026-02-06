#include "config.h"
#include "ColliderCmp.h"
#include "TransformCmp.h"
#include "render/physics.h"

void ColliderCmp::Start()
{
    transform = owner->transform;
}

void ColliderCmp::Update(float dt)
{
    Physics::SetTransform(*colId, transform->transform);
}