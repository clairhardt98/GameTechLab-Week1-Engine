﻿#pragma once

#include "Core/Engine.h"
#include "Object/USceneComponent.h"
#include "Primitive/PrimitiveVertices.h"


class UPrimitiveComponent : public USceneComponent
{
	using Super = USceneComponent;
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Render();


protected:
	EPrimitiveType PrimitiveType = EPrimitiveType::EPT_None;
	bool bCanBeRendered = PrimitiveType != EPrimitiveType::EPT_None;
};

class UCubeComp : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;

protected:
	EPrimitiveType PrimitiveType = EPrimitiveType::EPT_Cube;
};

class USphereComp : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;

protected:
	EPrimitiveType PrimitiveType = EPrimitiveType::EPT_Sphere;
};