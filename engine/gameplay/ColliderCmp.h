#pragma once
#include "../engine/core/ComponentBase.h"
#include <string>
#include <vector>

class ColliderCmp : public ComponentBase
{
public:
    glm::quat& orientation;

    void Start() override;
    void Update(float dt) override;

    void SetCollider(std::string meshPath);
    void SetCollider(std::vector<glm::vec3> endPoints, glm::vec3 center);
};