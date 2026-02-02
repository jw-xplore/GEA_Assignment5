#pragma once

class Entity;

class ComponentBase
{
public:
	Entity* owner;

	ComponentBase() {}
	virtual ~ComponentBase() {}

	virtual void Start() {}
	virtual void Update() {}
};