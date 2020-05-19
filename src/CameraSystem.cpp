#include "CameraSystem.h"
#include "extern.h"
#include "Game.h"

CameraSystem::CameraSystem()
{

}

bool CameraSystem::init()
{
    return true;
}

bool CameraSystem::lateInit()
{
    // Set default camera, the one to be used.
    int flyover_id = ECS.getEntity("camera_flyover");
    Entity& flyover = ECS.entities[flyover_id];
    SetDefaultCamera(flyover);

    // Output camera, where eerything is output too it.
    int main_camera_id = ECS.getEntity("camera_main");
    Entity& main_camera = ECS.entities[main_camera_id];
    SetOutputCamera(main_camera);

    ECS.main_camera = ECS.getComponentID<Camera>(flyover_id);

    return true;
}

bool CameraSystem::stop()
{
    return true;
}

void CameraSystem::update(float dt)
{
    updateTest(dt);

    Camera resultCamera;

    // Get default camera and set its transform
    if (_defaultCamera.isValid())
    {
        Entity e = _defaultCamera;
        Camera & c_camera = ECS.getComponentFromEntity<Camera>(e.name);
        blendCameras(&c_camera, &c_camera, 1.f, &resultCamera);

        Transform& c_trans = ECS.getComponentFromEntity<Transform>(e.name);
        c_trans.position(resultCamera.position);
    }
}

void CameraSystem::updateTest(float dt)
{
    // TO-DO
    // Add lerps and transitions between scene cameras
    // Create the cinematic code here.
}

void CameraSystem::render()
{

}

void CameraSystem::SetDefaultCamera(Entity & camera)
{
}

void CameraSystem::SetOutputCamera(Entity & camera)
{
}

int CameraSystem::GetOutputCamera()
{
    return 0;
}

int CameraSystem::GetDefaultCamera()
{
    return 0;
}

int CameraSystem::GetOutputCameraEntity()
{
    return 0;
}

int CameraSystem::GetDefaultCameraEntity()
{
    return 0;
}


// Start blending camera
void CameraSystem::blendInCamera(Entity camera, float blendTime, EPriority priority, Interpolator::IInterpolator* interpolator)
{
    CameraMixed* mc = getMixedCamera(camera);

    if (!mc)
    {
        CameraMixed new_mc;
        new_mc.camera = camera;
        new_mc.type = priority;
        new_mc.interpolator = interpolator;
        new_mc.blendIn(blendTime);

        _mixedCameras.push_back(new_mc);
    }
}

// End blending camera
void CameraSystem::blendOutCamera(Entity camera, float blendTime)
{
    CameraMixed* mc = getMixedCamera(camera);
    if (mc)
    {
        mc->blendOut(blendTime);
    }
}

// Blend the two cameras and output the result in the third one.
void CameraSystem::blendCameras(const Camera* camera1, const Camera* camera2, float ratio, Camera* output) const
{
    assert(camera1 && camera2 && output);

    // In case you want complex math function lerping, Apply interpolators instead.
    lm::vec3 newPos = lm::Utils::lerp(camera1->position, camera2->position, ratio);
    lm::vec3 newFront = lm::Utils::lerp(camera1->forward, camera2->forward, ratio);
    float newFov = lm::Utils::lerp(camera1->fov, camera2->fov, ratio);
    float newZnear = lm::Utils::lerp(camera1->near, camera2->near, ratio);
    float newZfar = lm::Utils::lerp(camera1->far, camera2->far, ratio);

    output->setPerspective(newFov, (float)Game::instance->window_width_ / (float)Game::instance->window_height_, newZnear, newZfar);
    output->lookAt(newPos, newPos + newFront);
}


// Mixed struct methods for mixed cameras

// Retrieve mixed camera
CameraSystem::CameraMixed* CameraSystem::getMixedCamera(Entity camera)
{
    for (auto& mc : _mixedCameras)
    {
        if (mc.camera.name == camera.name)
        {
            return &mc;
        }
    }

    return nullptr;
}

void CameraSystem::CameraMixed::blendIn(float duration)
{
    blendInTime = duration;
    state = blendInTime == 0.f ? ST_IDLE : ST_BLENDING_IN;
    time = 0.f;
}

void CameraSystem::CameraMixed::blendOut(float duration)
{
    blendOutTime = duration;
    state = ST_BLENDING_OUT;
    time = 0.f;
}

// UI Debug purposes
// Rendering the types of interpolation that we have in the ImGUI

// Used to render splines in imgui
void renderInterpolator(const char* name, Interpolator::IInterpolator& interpolator)
{
    const int nsamples = 50;
    float values[nsamples];

    for (int i = 0; i < nsamples; ++i)
        values[i] = interpolator.blend(0.f, 1.f, (float)i / (float)nsamples);

    ImGui::PlotLines(name, values, nsamples, 0, 0,
        std::numeric_limits<float>::min(), std::numeric_limits<float>::max(),
        ImVec2(150, 80));
}


// Display the different ways to interpolate camera transforms.
void CameraSystem::renderInMenu()
{

    if (ImGui::TreeNode("Interpolators"))
    {
        renderInterpolator("Linear", Interpolator::TLinearInterpolator());
        renderInterpolator("Quad in", Interpolator::TQuadInInterpolator());
        renderInterpolator("Quad out", Interpolator::TQuadOutInterpolator());
        renderInterpolator("Quad in out", Interpolator::TQuadInOutInterpolator());
        renderInterpolator("Cubic in", Interpolator::TCubicInInterpolator());
        renderInterpolator("Cubic out", Interpolator::TCubicOutInterpolator());
        renderInterpolator("Cubic in out", Interpolator::TCubicInOutInterpolator());
        renderInterpolator("Quart in", Interpolator::TQuartInInterpolator());
        renderInterpolator("Quart out", Interpolator::TQuartOutInterpolator());
        renderInterpolator("Quart in out", Interpolator::TQuartInOutInterpolator());
        renderInterpolator("Quint in", Interpolator::TQuintInInterpolator());
        renderInterpolator("Quint out", Interpolator::TQuintOutInterpolator());
        renderInterpolator("Quint in out", Interpolator::TQuintInOutInterpolator());
        renderInterpolator("Back in", Interpolator::TBackInInterpolator());
        renderInterpolator("Back out", Interpolator::TBackOutInterpolator());
        renderInterpolator("Back in out", Interpolator::TBackInOutInterpolator());
        renderInterpolator("Elastic in", Interpolator::TElasticInInterpolator());
        renderInterpolator("Elastic out", Interpolator::TElasticOutInterpolator());
        renderInterpolator("Elastic in out", Interpolator::TElasticInOutInterpolator());
        renderInterpolator("Bounce in", Interpolator::TBounceInInterpolator());
        renderInterpolator("Bounce out", Interpolator::TBounceOutInterpolator());
        renderInterpolator("Bounce in out", Interpolator::TBounceInOutInterpolator());
        renderInterpolator("Circular in", Interpolator::TCircularInInterpolator());
        renderInterpolator("Circular out", Interpolator::TCircularOutInterpolator());
        renderInterpolator("Circular in out", Interpolator::TCircularInOutInterpolator());
        renderInterpolator("Expo in", Interpolator::TExpoInInterpolator());
        renderInterpolator("Expo out", Interpolator::TExpoOutInterpolator());
        renderInterpolator("Expo in out", Interpolator::TExpoInOutInterpolator());
        renderInterpolator("Sine in", Interpolator::TSineInInterpolator());
        renderInterpolator("Sine out", Interpolator::TSineOutInterpolator());
        renderInterpolator("Sine in out", Interpolator::TSineInOutInterpolator());

        ImGui::TreePop();
    }
}
