#pragma once

class ComponentBase
{
public:
	//Entity* entity;

	ComponentBase() {}
	virtual ~ComponentBase() {}

	virtual unsigned int Id() { return 0; }
	virtual void Start() {}
	virtual void Update() {}
};