//------------------------------------------------------------------------------
// spacegameapp.cc
// (C) 2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "spacegameapp.h"
#include <cstring>
#include "imgui.h"
#include "render/renderdevice.h"
#include "render/shaderresource.h"
#include <vector>
#include "render/textureresource.h"
#include "render/model.h"
#include "render/cameramanager.h"
#include "render/lightserver.h"
#include "render/debugrender.h"
#include "core/random.h"
#include "render/input/inputserver.h"
#include "core/cvar.h"
#include "render/physics.h"
#include <chrono>
#include "spaceship.h"
#include "core/ECManager.h"
#include "core/MemoryManager.h"
#include "core/InputSystem.h"

#include "gameplay/TransformCmp.h"
#include "gameplay/RenderableCmp.h"
#include "gameplay/ShipCmp.h"
#include "gameplay/LigthCmp.h"
#include "gameplay/ColliderCmp.h"

using namespace Display;
using namespace Render;

namespace Game
{

//------------------------------------------------------------------------------
/**
*/
SpaceGameApp::SpaceGameApp()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SpaceGameApp::~SpaceGameApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
SpaceGameApp::Open()
{
	App::Open();
	this->window = new Display::Window;
    this->window->SetSize(2500, 2000);

    if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        RenderDevice::Init();

		// set ui rendering function
		this->window->SetUiRender([this]()
		{
			this->RenderUI();
		});
        
        return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
SpaceGameApp::Run()
{
    int w;
    int h;
    this->window->GetSize(w, h);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), float(w) / float(h), 0.01f, 1000.f);
    Camera* cam = CameraManager::GetCamera(CAMERA_MAIN);
    cam->projection = projection;

    InputSystem* inputSystem = InputSystem::GetInstance();

    // load all resources
    ModelId models[6] = {
        LoadModel("assets/space/Asteroid_1.glb"),
        LoadModel("assets/space/Asteroid_2.glb"),
        LoadModel("assets/space/Asteroid_3.glb"),
        LoadModel("assets/space/Asteroid_4.glb"),
        LoadModel("assets/space/Asteroid_5.glb"),
        LoadModel("assets/space/Asteroid_6.glb")
    };
    Physics::ColliderMeshId colliderMeshes[6] = {
        Physics::LoadColliderMesh("assets/space/Asteroid_1_physics.glb"),
        Physics::LoadColliderMesh("assets/space/Asteroid_2_physics.glb"),
        Physics::LoadColliderMesh("assets/space/Asteroid_3_physics.glb"),
        Physics::LoadColliderMesh("assets/space/Asteroid_4_physics.glb"),
        Physics::LoadColliderMesh("assets/space/Asteroid_5_physics.glb"),
        Physics::LoadColliderMesh("assets/space/Asteroid_6_physics.glb")
    };

    std::vector<std::tuple<ModelId, Physics::ColliderId, glm::mat4>> asteroids;

    ECManager ecManager;
    
    // Setup asteroids
    for (int i = 0; i < 150; i++)
    {
        // Rnd values
        size_t resourceIndex = (size_t)(Core::FastRandom() % 6);

        float span = 20.0f;
        if (i > 100)
            span = 80.0f;

        glm::vec3 translation = glm::vec3(
            Core::RandomFloatNTP() * span,
            Core::RandomFloatNTP() * span,
            Core::RandomFloatNTP() * span
        );

        glm::vec3 rotationAxis = normalize(translation);
        float rotation = translation.x;
        glm::mat4 transform = glm::rotate(rotation, rotationAxis) * glm::translate(translation);

        // Components setup
        TransformCmp* transformCmp = TransformCmp::Allocate();
        transformCmp->position = translation;

        float spinRnd = 1.0f;
        glm::vec3 spin = glm::vec3(
            Core::RandomFloatNTP() * spinRnd,
            Core::RandomFloatNTP() * spinRnd,
            Core::RandomFloatNTP() * spinRnd
        );

        transformCmp->angularVelocity = spin;

        RenderableCmp* renderableCmp = RenderableCmp::Allocate();
        renderableCmp->modelId = resourceIndex;
        
        ColliderCmp* colCmp = ColliderCmp::Allocate();
        Physics::ColliderId newCollider = Physics::CreateCollider(colliderMeshes[resourceIndex], transformCmp->transform);
        colCmp->colId = new Physics::ColliderId(newCollider.index, newCollider.generation);
        colCmp->colId->index = newCollider.index;

        // Add entity
        ecManager.AddEntity({
            transformCmp,
            renderableCmp,
            colCmp
            });
    }

    // Setup skybox
    std::vector<const char*> skybox
    {
        "assets/space/bg.png",
        "assets/space/bg.png",
        "assets/space/bg.png",
        "assets/space/bg.png",
        "assets/space/bg.png",
        "assets/space/bg.png"
    };
    TextureResourceId skyboxId = TextureResource::LoadCubemap("skybox", skybox, true);
    RenderDevice::SetSkybox(skyboxId);
    
    Input::Keyboard* kbd = Input::GetDefaultKeyboard();

    // Setup ship
    RenderableCmp* shipRender = RenderableCmp::Allocate();
    shipRender->modelId = LoadModel("assets/space/spaceship.glb");

    Entity ship = *ecManager.AddEntity({
        TransformCmp::Allocate(),
        new ShipCmp(),
        shipRender
        });


    const int numLights = 40;
    Entity* lightEntity;
    std::vector<Entity*> lightEntities;
    // Setup lights
    for (int i = 0; i < numLights; i++)
    {
        glm::vec3 translation = glm::vec3(
            Core::RandomFloatNTP() * 20.0f,
            Core::RandomFloatNTP() * 20.0f,
            Core::RandomFloatNTP() * 20.0f
        );
        glm::vec3 color = glm::vec3(
            Core::RandomFloat(),
            Core::RandomFloat(),
            Core::RandomFloat()
        );
        //lights[i] = Render::LightServer::CreatePointLight(translation, color, Core::RandomFloat() * 4.0f, 1.0f + (15 + Core::RandomFloat() * 10.0f));

        // Setup entity
        TransformCmp* transformCmp = TransformCmp::Allocate();
        transformCmp->position = translation;

        LigthCmp* light = LigthCmp::Allocate();
        light->color = color;
        light->intensity = Core::RandomFloat() * 4.0f;
        light->radius = 1.0f + (15 + Core::RandomFloat() * 10.0f);

        lightEntity = ecManager.AddEntity({
            transformCmp,
            light
            });

        lightEntities.push_back(lightEntity);
    }

    std::clock_t c_start = std::clock();
    double dt = 0.01667f;

    // game loop
    while (this->window->IsOpen())
	{
        auto timeStart = std::chrono::steady_clock::now();
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
        
        this->window->Update();

        if (kbd->pressed[Input::Key::Code::End])
        {
            ShaderResource::ReloadShaders();
        }

        // Draw some debug text
        Debug::DrawDebugText("FOOBAR", glm::vec3(0), {1,0,0,1});

        ecManager.Update(dt);

        // Execute the entire rendering pipeline
        RenderDevice::Render(this->window, dt);

		// transfer new frame to window
		this->window->SwapBuffers();

        auto timeEnd = std::chrono::steady_clock::now();
        dt = std::min(0.04, std::chrono::duration<double>(timeEnd - timeStart).count());

        if (kbd->pressed[Input::Key::Code::Escape])
            this->Exit();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
SpaceGameApp::Exit()
{
    this->window->Close();
}

//------------------------------------------------------------------------------
/**
*/
void
SpaceGameApp::RenderUI()
{
	if (this->window->IsOpen())
	{
        ImGui::Begin("Debug");
        Core::CVar* r_draw_light_spheres = Core::CVarGet("r_draw_light_spheres");
        int drawLightSpheres = Core::CVarReadInt(r_draw_light_spheres);
        if (ImGui::Checkbox("Draw Light Spheres", (bool*)&drawLightSpheres))
            Core::CVarWriteInt(r_draw_light_spheres, drawLightSpheres);
        
        Core::CVar* r_draw_light_sphere_id = Core::CVarGet("r_draw_light_sphere_id");
        int lightSphereId = Core::CVarReadInt(r_draw_light_sphere_id);
        if (ImGui::InputInt("LightSphereId", (int*)&lightSphereId))
            Core::CVarWriteInt(r_draw_light_sphere_id, lightSphereId);
        
        ImGui::End();

        Debug::DispatchDebugTextDrawing();
	}
}

} // namespace Game