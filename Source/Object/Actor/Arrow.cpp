#include "Arrow.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

AArrow::AArrow()
{
	bCanEverTick = true;

	UCylinderComp* CylinderComp = AddComponent<UCylinderComp>();
	RootComponent = CylinderComp;

	UConeComp* ConeComp = AddComponent<UConeComp>();
	ConeComp->SetPosition(FVector(0, 0, 1));

	ConeComp->SetupAttachment(RootComponent);
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const char* AArrow::GetTypeName()
{
	return "Arrow";
}
