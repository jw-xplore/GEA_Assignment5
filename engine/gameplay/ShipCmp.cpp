#include "config.h"
#include "ShipCmp.h"
#include "render/input/inputserver.h"
#include "render/cameramanager.h"
#include "render/physics.h"
#include "render/debugrender.h"
#include "render/particlesystem.h"
#include "core/InputSystem.h"

#include "TransformCmp.h"
#include "CameraCmp.h" 

using namespace Input;
using namespace glm;
using namespace Render;


void ShipCmp::Start()
{
    transform = owner->FindComponent<TransformCmp>();

    uint32_t numParticles = 2048;
    this->particleEmitterLeft = new ParticleEmitter(numParticles);
    this->particleEmitterLeft->data = {
        .origin = glm::vec4(transform->position + (vec3(transform->transform[2]) * emitterOffset),1),
        .dir = glm::vec4(glm::vec3(-transform->transform[2]), 0),
        .startColor = glm::vec4(0.38f, 0.76f, 0.95f, 1.0f) * 2.0f,
        .endColor = glm::vec4(0,0,0,1.0f),
        .numParticles = numParticles,
        .theta = glm::radians(0.0f),
        .startSpeed = 1.2f,
        .endSpeed = 0.0f,
        .startScale = 0.025f,
        .endScale = 0.0f,
        .decayTime = 2.58f,
        .randomTimeOffsetDist = 2.58f,
        .looping = 1,
        .emitterType = 1,
        .discRadius = 0.020f
    };
    this->particleEmitterRight = new ParticleEmitter(numParticles);
    this->particleEmitterRight->data = this->particleEmitterLeft->data;

    ParticleSystem::Instance()->AddEmitter(this->particleEmitterLeft);
    ParticleSystem::Instance()->AddEmitter(this->particleEmitterRight);

    camera = new CameraCmp();
    camera->Init(CameraManager::GetCamera(CAMERA_MAIN), transform);
    this->owner->AddComponent(camera);

    // Setup input
    inputSystem = InputSystem::GetInstance();
}

void ShipCmp::Update(float dt)
{
    CheckCollisions();

    float forwardInput = inputSystem->actions["Forward"]->InputAxis();

    if (forwardInput)
    {
        if (inputSystem->actions["Boost"]->IsPressed())
            this->currentSpeed = mix(this->currentSpeed, this->boostSpeed * forwardInput, std::min(1.0f, dt * 30.0f));
        else
            this->currentSpeed = mix(this->currentSpeed, this->normalSpeed * forwardInput, std::min(1.0f, dt * 90.0f));
    }
    else
    {
        this->currentSpeed = 0;
    }

    vec3 desiredVelocity = vec3(0, 0, this->currentSpeed);
    desiredVelocity = transform->transform * vec4(desiredVelocity, 0.0f);

    transform->linearVelocity = mix(transform->linearVelocity, desiredVelocity, dt * accelerationFactor);

    float rotX = inputSystem->actions["Yaw"]->InputAxis();
    float rotY = inputSystem->actions["Pitch"]->InputAxis();
    float rotZ = inputSystem->actions["Roll"]->InputAxis();

    transform->position += transform->linearVelocity * dt * 10.0f;

    const float rotationSpeed = 1.8f * dt;
    rotXSmooth = mix(rotXSmooth, rotX * rotationSpeed, dt * cameraSmoothFactor);
    rotYSmooth = mix(rotYSmooth, rotY * rotationSpeed, dt * cameraSmoothFactor);
    rotZSmooth = mix(rotZSmooth, rotZ * rotationSpeed, dt * cameraSmoothFactor);
    quat localOrientation = quat(vec3(-rotYSmooth, rotXSmooth, rotZSmooth));
    transform->orientation = transform->orientation * localOrientation;
    this->rotationZ -= rotXSmooth;
    this->rotationZ = clamp(this->rotationZ, -45.0f, 45.0f);
    mat4 T = translate(transform->position) * (mat4)transform->orientation;
    transform->transform = T * (mat4)quat(vec3(0, 0, rotationZ));
    this->rotationZ = mix(this->rotationZ, 0.0f, dt * cameraSmoothFactor);

    // update camera view transform
    vec3 desiredCamPos = transform->position + vec3(transform->transform * vec4(0, camOffsetY, -4.0f, 0));
    camera->UpdateTransform(dt, desiredCamPos, cameraSmoothFactor);
    //this->camPos = mix(this->camPos, desiredCamPos, dt * cameraSmoothFactor);
    //cam->view = lookAt(this->camPos, this->camPos + vec3(transform->transform[2]), vec3(transform->transform[1]));

    const float thrusterPosOffset = 0.365f;
    this->particleEmitterLeft->data.origin = glm::vec4(vec3(transform->position + (vec3(transform->transform[0]) * -thrusterPosOffset)) + (vec3(transform->transform[2]) * emitterOffset), 1);
    this->particleEmitterLeft->data.dir = glm::vec4(glm::vec3(-transform->transform[2]), 0);
    this->particleEmitterRight->data.origin = glm::vec4(vec3(transform->position + (vec3(transform->transform[0]) * thrusterPosOffset)) + (vec3(transform->transform[2]) * emitterOffset), 1);
    this->particleEmitterRight->data.dir = glm::vec4(glm::vec3(-transform->transform[2]), 0);

    float t = (currentSpeed / this->normalSpeed);
    this->particleEmitterLeft->data.startSpeed = 1.2 + (3.0f * t);
    this->particleEmitterLeft->data.endSpeed = 0.0f + (3.0f * t);
    this->particleEmitterRight->data.startSpeed = 1.2 + (3.0f * t);
    this->particleEmitterRight->data.endSpeed = 0.0f + (3.0f * t);
    //this->particleEmitter->data.decayTime = 0.16f;//+ (0.01f  * t);
    //this->particleEmitter->data.randomTimeOffsetDist = 0.06f;/// +(0.01f * t);
}

bool ShipCmp::CheckCollisions()
{
    glm::mat4 rotation = (glm::mat4)transform->orientation;
    bool hit = false;
    for (int i = 0; i < sizeof(colliderEndPoints) / sizeof(glm::vec3); i++)
    {
        glm::vec3 pos = transform->position;
        glm::vec3 dir = transform->transform * glm::vec4(glm::normalize(colliderEndPoints[i]), 0.0f);
        float len = glm::length(colliderEndPoints[i]);
        Physics::RaycastPayload payload = Physics::Raycast(transform->position, dir, len);

        // debug draw collision rays
        Debug::DrawLine(pos, pos + dir * len, 1.0f, glm::vec4(0, 1, 0, 1), glm::vec4(0, 1, 0, 1), Debug::RenderMode::AlwaysOnTop);

        if (payload.hit)
        {
            Debug::DrawDebugText("HIT", payload.hitPoint, glm::vec4(1, 1, 1, 1));
            hit = true;
        }
    }
    return hit;
}