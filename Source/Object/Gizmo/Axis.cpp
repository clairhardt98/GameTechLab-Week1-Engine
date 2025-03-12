#include "Axis.h"

AAxis::AAxis()
{
	bIsGizmo = true;

	ULineComp* LineX = AddComponent<ULineComp>();
	FTransform XTransform = LineX->GetComponentTransform();
	XTransform.SetScale(FVector(1000.0f, 1.0f, 1.0f));
	XTransform.Rotate({0.0f, 0.0f, 0.0f});
	LineX->SetRelativeTransform(XTransform);
	LineX->SetCustomColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));

	FVector Euler = LineX->GetComponentTransform().GetRotation().GetEuler();
	UE_LOG("LineX Rot: %f, %f, %f", 
		Euler.X,
		Euler.Y,
		Euler.Z
	);

	RootComponent = LineX;

	ULineComp* LineY = AddComponent<ULineComp>();
	FTransform YTransform = LineY->GetComponentTransform();
	YTransform.SetScale(FVector(1000.0f, 1.0f, 1.0f));
	YTransform.Rotate({0.0f, 0.0f, -90.0f}); // z기준 시계로 90도 회전
	LineY->SetRelativeTransform(YTransform);
	LineY->SetCustomColor(FVector4(0.0f, 1.0f, 0.0f, 1.0f));


	Euler = LineY->GetComponentTransform().GetRotation().GetEuler();

	ULineComp* LineZ = AddComponent<ULineComp>();
	FTransform ZTransform = LineZ->GetComponentTransform();
	ZTransform.SetScale(FVector(1000.0f, 1.0f, 1.0f));
	ZTransform.Rotate({0.0f, 90.0f, 0.0f}); // y기준 반시계로 90도 회전
	LineZ->SetRelativeTransform(ZTransform);
	LineZ->SetCustomColor(FVector4(0.0f, 0.0f, 1.0f, 1.0f));

	Euler = LineZ->GetComponentTransform().GetRotation().GetEuler();
} 

void AAxis::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAxis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const char* AAxis::GetTypeName()
{
	return "Axis";
}
