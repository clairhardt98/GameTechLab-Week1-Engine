#include "UPrimitiveComponent.h"
#include "Object/World/World.h"
#include "Object/Actor/Actor.h"


UPrimitiveComponent::UPrimitiveComponent()
{
}

void UPrimitiveComponent::BeginPlay()
{
	Super::BeginPlay();
	InitBoundingBox();
	GetOwner()->GetWorld()->AddBoundingBox(&BoundingBox);
}

void UPrimitiveComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

void UPrimitiveComponent::UpdateConstantPicking(const URenderer& Renderer, const FVector4 UUIDColor)const
{
	Renderer.UpdateConstantPicking(UUIDColor);
}

void UPrimitiveComponent::UpdateConstantDepth(const URenderer& Renderer, const int Depth)const
{
	Renderer.UpdateConstantDepth(Depth);
}

void UPrimitiveComponent::Render()
{
	URenderer* Renderer = UEngine::Get().GetRenderer();
	if (Renderer == nullptr || !bCanBeRendered)
	{
		return;
	}
	if (GetOwner()->IsGizmoActor() == false)
	{
		if (bIsPicked)
		{
			/*bUseVertexColor = false;
			SetCustomColor(FVector4(1.0f, 0.647f, 0.0f, 1.0f));*/
		}
		else
		{
			bUseVertexColor = true;
		}
	}
	Renderer->RenderPrimitive(this);
}

void UPrimitiveComponent::SetRelativeTransform(const FTransform& InTransform)
{
	Super::SetRelativeTransform(InTransform);
	UpdateBoundingBox();
}

void UPrimitiveComponent::RegisterComponentWithWorld(UWorld* World)
{
	World->AddRenderComponent(this);
}

void UPrimitiveComponent::InitBoundingBox()
{
	// 버텍스 정보 가져와서 AABB생성
	Super::InitBoundingBox();
}

void UPrimitiveComponent::UpdateBoundingBox()
{
	Super::InitBoundingBox();

	FTransform Transform = GetWorldTransform();
	BoundingBox.Update(Transform.GetMatrix());
}
