#pragma once
#include "../engine/core/ComponentBase.h"
#include "core/MemoryManager.h"

namespace Render
{
	struct Camera;
}

class TransformCmp;

class CameraCmp : public ComponentBase
{
public:
	const static int CMPID = 5;
	Render::Camera* camera;
	TransformCmp* transform;

	glm::vec3 camPos;

	void Start() override {}
	void Update(float dt) override {}

	void Init(Render::Camera* camera, TransformCmp* transform);
	void UpdateTransform(float dt, glm::vec3 transform, float smoothFactor);
	virtual int GetId() override { return CMPID; }
};