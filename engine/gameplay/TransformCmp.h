#pragma once
#include "../engine/core/ComponentBase.h"

class TransformCmp : public ComponentBase
{
public:
    glm::vec3 position = glm::vec3(0);
    glm::quat orientation = glm::identity<glm::quat>();
    glm::vec3 camPos = glm::vec3(0, 1.0f, -2.0f);
    glm::mat4 transform = glm::mat4(1);
    glm::vec3 linearVelocity = glm::vec3(0);
    glm::vec3 angularVelocity = glm::vec3(0);

    TransformCmp() {}
    TransformCmp(glm::mat4 transform);

    void Start() override;
    void Update(float dt) override;
};