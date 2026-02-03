#include "config.h"
#include "ColliderCmp.h"
#include "TransformCmp.h"

void ColliderCmp::Start()
{
    transform = owner->FindComponent<TransformCmp>();
}

void ColliderCmp::Update(float dt)
{
    glm::mat4 rotation = (glm::mat4)transform->orientation;
    
    /*
    for (int i = 0; i < sizeof(colliderEndPoints) / sizeof(glm::vec3); i++)
    {
        glm::vec3 pos = position;
        glm::vec3 dir = this->transform * glm::vec4(glm::normalize(colliderEndPoints[i]), 0.0f);
        float len = glm::length(colliderEndPoints[i]);
        Physics::RaycastPayload payload = Physics::Raycast(position, dir, len);

        // debug draw collision rays
        Debug::DrawLine(pos, pos + dir * len, 1.0f, glm::vec4(0, 1, 0, 1), glm::vec4(0, 1, 0, 1), Debug::RenderMode::AlwaysOnTop);

        if (payload.hit)
        {
            Debug::DrawDebugText("HIT", payload.hitPoint, glm::vec4(1, 1, 1, 1));
        }
    }
    */
}

void ColliderCmp::SetCollider(Physics::ColliderId* id)
{

}

void ColliderCmp::SetCollider(std::vector<glm::vec3> endPoints, glm::vec3 center)
{

}