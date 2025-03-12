#include "Camera.h"

#include "Core/Rendering/URenderer.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"


ACamera::ACamera()
{
    bIsGizmo = true;
    
    Near = 0.1f;
    Far = 100.f;
    FieldOfView = 90.f;
    ProjectionMode = ECameraProjectionMode::Perspective;

    RootComponent = AddComponent<USceneComponent>();
    
    FTransform StartPos = GetActorTransform();
    StartPos.SetPosition(FVector(-5, 0, 1));
    //StartPos.LookAt(FVector(0, 0, 0), GetForward());

    //FVector Rotation = StartPos.GetRotation().GetEuler();
    //StartPos.SetRotation(0, Rotation.Y, Rotation.Z);
    //
    SetActorTransform(StartPos);
}

void ACamera::SetFieldOfVew(float Fov)
{
    FieldOfView = Fov;
}

void ACamera::SetFar(float Far)
{
    this->Far = Far;
}

void ACamera::SetNear(float Near)
{
    this->Near = Near;
}

float ACamera::GetFieldOfView() const
{
    return  FieldOfView;
}

float ACamera::GetNear() const
{
    return Near;
}

float ACamera::GetFar() const
{
    return Far;
}
