#include "config.h"
#include "CameraCmp.h"
#include "TransformCmp.h"
#include "render/cameramanager.h"

void CameraCmp::Init(Render::Camera* camera, TransformCmp* transform)
{
    this->camera = camera;
    this->transform = transform;
}

void CameraCmp::UpdateTransform(float dt, glm::vec3 pos, float smoothFactor)
{
    using namespace glm;

    //vec3 camPos = mix(transform->position, desiredCamPos, dt * smoothFactor);
    //camera->view = lookAt(camPos, camPos + vec3(transform->transform[2]), vec3(transform->transform[1]));
}