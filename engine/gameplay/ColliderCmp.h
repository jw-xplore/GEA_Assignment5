#pragma once
#include "../engine/core/ComponentBase.h"
#include <string>
#include <vector>
#include "core/MemoryManager.h"

class TransformCmp;

namespace Physics
{
    struct ColliderId;
}

class ColliderCmp : public ComponentBase
{
public:
    const static int CMPID = 2;
    Physics::ColliderId* colId;
    TransformCmp* transform;

    void Start() override;
    void Update(float dt) override;
    virtual int GetId() override { return CMPID; }

    // Createre and remove
    static PoolAllocator<ColliderCmp> allocator;

    static ColliderCmp* Allocate() { return ColliderCmp::allocator.Allocate(); }
    void Deallocate() override { ColliderCmp::allocator.Remove(this); }
};