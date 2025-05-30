﻿#include "USceneComponent.h"
#include "Debug/DebugConsole.h"
#include "PrimitiveComponent/UPrimitiveComponent.h"

void USceneComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USceneComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 내 월드 트랜스폼 반환
const FTransform USceneComponent::GetWorldTransform()
{
	if (Parent)
	{
		// 부모가 있을 경우 부모 월드 * 내 로컬
		FMatrix ParentWorld = Parent->GetWorldTransform().GetMatrix();
		FMatrix MyLocal = RelativeTransform.GetMatrix();

		FMatrix NewMatrix = MyLocal * ParentWorld;
		return NewMatrix.GetTransform();
	}

	return RelativeTransform;
}

void USceneComponent::SetRelativeTransform(const FTransform& InTransform)
{
	// 내 로컬 트랜스폼 갱신
	RelativeTransform = InTransform;
	// 바운딩박스 갱신
	BoundingBox.Update(RelativeTransform.GetMatrix());
}

void USceneComponent::SetScale(const FVector& InScale)
{
	RelativeTransform.SetScale(InScale);
}

void USceneComponent::SetPosition(const FVector& InPosition)
{
	RelativeTransform.SetPosition(InPosition);
}

void USceneComponent::SetRotation(const FVector& InRotation)
{
	RelativeTransform.SetRotation(InRotation);
}

void USceneComponent::SetRotation(const FQuaternion& InRotation)
{
	RelativeTransform.SetRotation(InRotation);
}

void USceneComponent::Pick(bool bPicked)
{
	bIsPicked = bPicked;
	for (auto& Child : Children)
	{
		Child->Pick(bPicked);
	}
}

void USceneComponent::SetupAttachment(USceneComponent* InParent, bool bUpdateChildTransform)
{
	if (InParent)
	{
		Parent = InParent;
		InParent->Children.Add(this);
		ApplyParentWorldTransform(InParent->GetWorldTransform());
	}
	else
	{
		UE_LOG("Parent is nullptr");
	}
}

void USceneComponent::ApplyParentWorldTransform(const FTransform& ParentWorldTransform)
{
	FMatrix ParentWorld = ParentWorldTransform.GetMatrix();
	FMatrix MyLocal = RelativeTransform.GetMatrix();

	FMatrix NewMatrix = MyLocal * ParentWorld.Inverse();

	// 내 로컬 트랜스폼 갱신
	SetRelativeTransform(NewMatrix.GetTransform());
}
