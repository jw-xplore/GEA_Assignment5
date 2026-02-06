#include "config.h"
#include "LigthCmp.h"
#include "render/lightserver.h"
#include "TransformCmp.h"

void LigthCmp::Start()
{
	Render::LightServer::CreatePointLight(owner->transform->position, color, intensity, radius);
}