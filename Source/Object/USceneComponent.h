#pragma once
#include "UObject.h"
#include "Core/Math/Vector.h"
#include "Core/Container/Set.h"
#include "Core/Math/Transform.h"
#include "Core/Math/Matrix.h"
#include "Object/ActorComponent/ActorComponent.h"

class USceneComponent : public UActorComponent
{
	friend class AActor;
	using Super = UActorComponent;
public:
	USceneComponent() = default;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Pick(bool bPicked);

public:
	/* 로컬 트랜스폼을 반환*/
	FTransform GetComponentTransform() const { return RelativeTransform; }
	/* 월드 트랜스폼을 반환, 이걸로 렌더링한다*/
	const FTransform GetWorldTransform();
	virtual void SetRelativeTransform(const FTransform& InTransform);

	void SetScale(const FVector& InScale);
	void SetPosition(const FVector& InPosition);
	void SetRotation(const FVector& InRotation);
	void SetRotation(const struct FQuaternion& InRotation);

public:
	bool IsPicked() const { return bIsPicked; }

public:
	void SetupAttachment(USceneComponent* InParent, bool bUpdateChildTransform = false);
	// 부모의 월드 트랜스폼을 받아서 자신의 로컬 트랜스폼을 갱신
	void ApplyParentWorldTransform(const FTransform& InTransform);

protected:
	virtual void InitBoundingBox() {}
	virtual void UpdateBoundingBox() {}

protected:
	USceneComponent* Parent = nullptr;
	TSet<USceneComponent*> Children;
	// 이건 내 로컬 트랜스폼
	FTransform RelativeTransform = FTransform();
	bool bCanEverTick = true;

	// 바운딩 박스
	FBox BoundingBox;
	// debug
protected:
	bool bIsPicked = false;
};