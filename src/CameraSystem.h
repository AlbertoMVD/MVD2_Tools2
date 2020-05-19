#pragma once
#include "includes.h"
#include "Components.h"
#include "interpolators.h"

class CameraSystem 
{
public:

    // Cameras have priorities to render on top of another depending on these.
    enum EPriority { DEFAULT = 0, GAMEPLAY, TEMPORARY, DEBUG, NUM_PROPERTIES };
    CameraSystem();

    bool init();
    bool lateInit();
    bool stop();
    void update(float dt);
    void updateTest(float dt);

    void render();

    // Brain and virtual cinemachine model, default(brain) and output(virtual) camera
    void SetDefaultCamera(Entity & camera);
    void SetOutputCamera(Entity & camera);

    int  GetOutputCamera();
    int  GetDefaultCamera();
    int  GetOutputCameraEntity();
    int  GetDefaultCameraEntity();

    // Blending camera functions
    void blendCameras(const Camera* camera1, const Camera* camera2, float ratio, Camera* output) const;
    void blendInCamera(Entity camera, float blendTime = 0.f, EPriority priority = EPriority::DEFAULT, Interpolator::IInterpolator* interpolator = nullptr);
    void blendOutCamera(Entity camera, float blendTime = 0.f);

    // ImGUI stuff related.
    void renderInMenu();

private:

    Entity _defaultCamera;
    Entity _outputCamera;

    // Camera type for transitions, temporal dummy camera
    struct CameraMixed
    {
        enum EState { ST_BLENDING_IN, ST_IDLE, ST_BLENDING_OUT };

        Entity camera;
        EState state = EState::ST_IDLE;
        EPriority type = EPriority::DEFAULT;

        float blendInTime = 0.f; // gets to full ratio (1.f) in n seconds
        float blendOutTime = 0.f; // gets to full ratio (1.f) in n seconds
        float weight = 0.f;  // blend weight ratio
        float time = 0.f; // current blending time

        // Type of interpolation for the camera
        Interpolator::IInterpolator* interpolator = nullptr;

        void blendIn(float duration);
        void blendOut(float duration);
    };

    std::vector<CameraMixed> _mixedCameras;

    CameraMixed* getMixedCamera(Entity camera);
};

