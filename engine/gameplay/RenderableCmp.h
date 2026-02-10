#pragma once
#include "../engine/core/ComponentBase.h"
#include "core/MemoryManager.h"

namespace Render
{
    using ModelId = uint32_t;
}

class TransformCmp;

class RenderableCmp : public ComponentBase
{
public:
    const static int CMPID = 1;
    TransformCmp* transform;
    Render::ModelId modelId;

    RenderableCmp() { modelId = 1; }
    RenderableCmp(Render::ModelId id);

    void Start() override;
    void Update(float dt) override;
    virtual int GetId() override { return CMPID; }

    // Createre and remove
    static PoolAllocator<RenderableCmp> allocator;

    static RenderableCmp* Allocate() { return RenderableCmp::allocator.Allocate(); }
    void Deallocate() override { RenderableCmp::allocator.Remove(this); }
};