#include "config.h"
#include "RenderableCmp.h"
#include "../render/renderdevice.h"
#include "TransformCmp.h"

RenderableCmp::RenderableCmp(Render::ModelId id)
{
	modelId = id;
}

void RenderableCmp::Start()
{
	transform = owner->transform;
}

void RenderableCmp::Update(float dt)
{
	using namespace Render;
	RenderDevice::Draw(modelId, transform->transform);
}