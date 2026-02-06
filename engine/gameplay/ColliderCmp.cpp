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
    //int a = 0;
    //glm::mat4 rotation = (glm::mat4)transform->orientation;
    Physics::SetTransform(*colId, transform->transform);
}

void ColliderCmp::SetCollider(Physics::ColliderId* id)
{

}

void ColliderCmp::SetCollider(std::vector<glm::vec3> endPoints, glm::vec3 center)
{

}