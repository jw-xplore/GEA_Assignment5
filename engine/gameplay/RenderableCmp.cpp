#include "config.h"
#include "RenderableCmp.h"

void RenderableCmp::Update(float dt)
{
	this->position += this->linearVelocity * (float)dt;
}