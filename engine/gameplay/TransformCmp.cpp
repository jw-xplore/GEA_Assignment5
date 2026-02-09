#include "config.h"
#include "TransformCmp.h"

TransformCmp::TransformCmp(glm::mat4 transform)
{
	this->transform = transform;
}

void TransformCmp::Start()
{

}

void TransformCmp::Update(float dt)
{
	using namespace glm;

	this->position += this->linearVelocity * dt;
	glm::quat quatAV = glm::quat(glm::vec3(-angularVelocity.y * dt, angularVelocity.x * dt, angularVelocity.z *dt));
	this->orientation = this->orientation * quatAV;

	glm::vec3 rotationAxis = normalize(position);
	float rotation = quatAV.x;

	transform = translate(this->position) * (mat4)this->orientation;
}