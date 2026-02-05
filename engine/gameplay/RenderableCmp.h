#pragma once
#include "../engine/core/ComponentBase.h"

namespace Render
{
    using ModelId = uint32_t;
}

class TransformCmp;

class RenderableCmp : public ComponentBase
{
public:
    TransformCmp* transform;
    Render::ModelId modelId;

    RenderableCmp() { modelId = 1; }
    RenderableCmp(Render::ModelId id);

    void Start() override;
    void Update(float dt) override;
};