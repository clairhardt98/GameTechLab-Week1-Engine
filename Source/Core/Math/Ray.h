#pragma once
#include "Vector.h"

struct FRay
{
public:
	~FRay() = default;

	FVector Origin;
	FVector Direction;

	FRay()
	{
		Init();
	}

	void Init()
	{
		Origin = FVector();
		Direction = FVector(0, 0, 1);
	}

	bool operator==(const FRay& Other) const
	{
		return Origin == Other.Origin && Direction == Other.Direction;
	}

	bool operator!=(const FRay& Other) const
	{
		return !(*this == Other);
	}

	FVector PointAt(float T) const
	{
		return Origin + Direction * T;
	}

	float Dist() const
	{
		return Direction.Length();
	}

	float DistSquared() const
	{
		return Direction.LengthSquared();
	}
};
