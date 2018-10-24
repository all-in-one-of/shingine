#include "Systems/CPlayerControllerSystem.h"
#include "Modules/Statics/CActiveCamera.h"
REGISTER_SERIALIZED_NAME(CPlayerControllerSystem, PlayerControllerSystem)

bool CPlayerControllerSystem::Initialize()
{
    CachedCamera = CActiveCamera::Get()->GetCameraComponent();
    return true;
}

bool CPlayerControllerSystem::Update()
{
    return true;
}

CPlayerControllerSystem::~CPlayerControllerSystem()
{
}