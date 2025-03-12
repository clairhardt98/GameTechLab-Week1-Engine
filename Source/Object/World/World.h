#pragma once
#include "JsonSavehelper.h"
#include "Core/Engine.h"
#include "Core/Container/Array.h"
#include "Core/Container/Set.h"
#include "Core/Container/String.h"
#include "Object/UObject.h"
#include "Debug/DebugConsole.h"
#include "Object/ObjectFactory.h"
#include "Object/Actor/Arrow.h"
#include "Object/Actor/Picker.h"
#include "Core/Math/Box.h"

class AActor;

class UWorld :public UObject
{
public:
	UWorld() = default;
	virtual ~UWorld() = default;

public:
	void BeginPlay();
	void Tick(float DeltaTime);
	void LateTick(float DeltaTime);

	template <typename T>
		requires std::derived_from<T, AActor>
	T* SpawnActor()
	{
		T* Actor = FObjectFactory::ConstructObject<T>();

		if (UWorld* World = UEngine::Get().GetWorld())
		{
			Actor->SetWorld(World);
			Actors.Add(Actor);
			ActorsToSpawn.Add(Actor);
			return Actor;
		}

		UE_LOG("Actor Construction Failed. World is nullptr");
		return nullptr;
	}

	bool DestroyActor(AActor* InActor);

	void Render();
	void RenderPickingTexture(const URenderer& Renderer);
	void DisplayPickingTexture(URenderer& Renderer);
	void RenderMainTexture(URenderer& Renderer);
	void RenderBoundingBoxes(URenderer& Renderer);

	void ClearWorld();
	void LoadWorld(const char* SceneName);
	void SaveWorld();

	void AddZIgnoreComponent(class UPrimitiveComponent* InComponent);
	void RemoveZIgnoreComponent(class UPrimitiveComponent* InComponent) { ZIgnoreRenderComponents.Remove(InComponent); }
	bool ContainsZIgnoreComponent(class UPrimitiveComponent* InComponent) { return ZIgnoreRenderComponents.Find(InComponent) != -1; }

	// render
	void AddRenderComponent(class UPrimitiveComponent* Component) { RenderComponents.Add(Component); }
	void RemoveRenderComponent(class UPrimitiveComponent* Component) { RenderComponents.Remove(Component); }

public:
	// linetrace
	bool LineTrace(const FVector& Start, const FVector& End, USceneComponent** FirstHitComponent) const;
	void AddBoundingBox(FBox* Box) { BoundingBoxes.Add(Box); }
	void RemoveBoundingBox(FBox* Box) { BoundingBoxes.Remove(Box); }


private:
	UWorldInfo GetWorldInfo() const;

public:
	FString SceneName;
	uint32 Version = 1;

protected:
	TArray<AActor*> Actors;
	TArray<UPrimitiveComponent*> ZIgnoreRenderComponents;
	TArray<AActor*> ActorsToSpawn;
	TArray<AActor*> PendingDestroyActors; // TODO: 추후에 TQueue로 변경
	TSet<UPrimitiveComponent*> RenderComponents;
	TSet<FBox*> BoundingBoxes;
};
