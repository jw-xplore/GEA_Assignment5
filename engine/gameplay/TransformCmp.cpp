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

	//glm::mat4 transform = glm::rotate(rotation, rotationAxis) * glm::translate(position);

	//glm::quat localOrientation = glm::quat(glm::vec3(-rotYSmooth, rotXSmooth, rotZSmooth));
	//this->orientation = this->orientation * localOrientation;
	//this->rotationZ -= rotXSmooth;
	///this->rotationZ = clamp(this->rotationZ, -45.0f, 45.0f);
	transform = translate(this->position) * (mat4)this->orientation;
	//this->transform = T;
}