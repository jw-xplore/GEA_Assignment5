#pragma once
#include "../engine/core/ComponentBase.h"

class LigthCmp : public ComponentBase
{
public:
	const static int CMPID = 3;
	glm::vec3 color;
	float intensity;
	float radius;

	void Start() override;
	void Update(float dt) override {}
	virtual int GetId() override { return CMPID; }
};