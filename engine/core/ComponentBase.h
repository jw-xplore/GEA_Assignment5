#pragma once
#include "Entity.h"

class ComponentBase
{
public:
	Entity* owner;

	ComponentBase() {}
	virtual ~ComponentBase() {}

	virtual void Start() {}
	virtual void Update(float dt) {}
	virtual int GetId() { return -1; }
};