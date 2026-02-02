#include "config.h"
#include "TransformCmp.h"

void TransformCmp::Update(float dt)
{
	this->position += this->linearVelocity * (float)dt;
}