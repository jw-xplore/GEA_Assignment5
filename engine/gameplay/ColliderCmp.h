#pragma once
#include "../engine/core/ComponentBase.h"
#include <string>
#include <vector>

class TransformCmp;

namespace Physics
{
    struct ColliderId;
}

class ColliderCmp : public ComponentBase
{
public:
    const static int CMPID = 0;
    Physics::ColliderId* colId;
    TransformCmp* transform;

    void Start() override;
    void Update(float dt) override;
    virtual int GetId() override { return CMPID; }

    //void SetCollider(std::string meshPath);
    void SetCollider(Physics::ColliderId* id);
    void SetCollider(std::vector<glm::vec3> endPoints, glm::vec3 center);
};