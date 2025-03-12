#include "Core/HAL/PlatformType.h"
#include "Core/Rendering/URenderer.h"
#include "Picker.h"

#include "Core/Input/PlayerInput.h"
#include "Object/Gizmo/GizmoHandle.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Static/FEditorManager.h"
#include "Object/Actor/Camera.h"

APicker::APicker()
{    
    bIsGizmo = true;
}

FVector4 APicker::EncodeUUID(unsigned int UUID)
{
    float a = (UUID >> 24) & 0xff;
    float b = (UUID >> 16) & 0xff;
    float g = (UUID >> 8) & 0xff;
    float r = UUID & 0xff;
	
    FVector4 color = {r, g, b, a};
    
    return color;
}

int APicker::DecodeUUID(FVector4 color)
{
    return (static_cast<unsigned int>(color.W)<<24) | (static_cast<unsigned int>(color.Z)<<16) | (static_cast<unsigned int>(color.Y)<<8) | (static_cast<unsigned int>(color.X));
}

void APicker::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APicker::LateTick(float DeltaTime)
{
    AActor::LateTick(DeltaTime);


    if(UInputManager::Get().GetMouseDown(false))
    {
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(UEngine::Get().GetWindowHandle(), &pt);

        float ratioX = UEngine::Get().GetInitializedScreenWidth() / (float)UEngine::Get().GetScreenWidth();
        float ratioY = UEngine::Get().GetInitializedScreenHeight() / (float)UEngine::Get().GetScreenHeight();
        pt.x = pt.x * ratioX;
        pt.y = pt.y * ratioY;
        
        FVector4 color = UEngine::Get().GetRenderer()->GetPixel(FVector(pt.x, pt.y, 0));

        uint32_t UUID = DecodeUUID(color);

        UActorComponent* PickedComponent = UEngine::Get().GetObjectByUUID<UActorComponent>(UUID);

        if (PickedComponent != nullptr)
        {
            AActor* PickedActor = PickedComponent->GetOwner();

            if (PickedActor == nullptr) return;
            if (PickedComponent->GetOwner()->IsGizmoActor() == false)
            {
                if (PickedActor == FEditorManager::Get().GetSelectedActor())
                {
                    FEditorManager::Get().SelectActor(nullptr);   
                }
                else
                {
                    FEditorManager::Get().SelectActor(PickedActor);
                }
            }
        }
        UE_LOG("Pick - UUID: %d", UUID);
    }
    if (UInputManager::Get().IsPressedMouse(false))
    {
        PickObjectByColorPixel();
    }
    else
    {
        if (AGizmoHandle* Handle = FEditorManager::Get().GetGizmoHandle())
        {
            Handle->SetSelectedAxis(ESelectedAxis::None);
        }
    }
}

const char* APicker::GetTypeName()
{
    return "Picker";
}

void APicker::PickObjectByColorPixel()
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(UEngine::Get().GetWindowHandle(), &pt);
    FVector4 color = UEngine::Get().GetRenderer()->GetPixel(FVector(pt.x, pt.y, 0));
    uint32_t UUID = DecodeUUID(color);

    UActorComponent* PickedComponent = UEngine::Get().GetObjectByUUID<UActorComponent>(UUID);
    if (PickedComponent != nullptr)
    {
        if (AGizmoHandle* Gizmo = dynamic_cast<AGizmoHandle*>(PickedComponent->GetOwner()))
        {
            if (Gizmo->GetSelectedAxis() != ESelectedAxis::None) return;
            UCylinderComp* CylinderComp = static_cast<UCylinderComp*>(PickedComponent);
            FVector4 CompColor = CylinderComp->GetCustomColor();
            if (1.0f - FMath::Abs(CompColor.X) < KINDA_SMALL_NUMBER) // Red - X축
            {
                Gizmo->SetSelectedAxis(ESelectedAxis::X);
            }
            else if (1.0f - FMath::Abs(CompColor.Y) < KINDA_SMALL_NUMBER) // Green - Y축
            {
                Gizmo->SetSelectedAxis(ESelectedAxis::Y);
            }
            else  // Blue - Z축
            {
                Gizmo->SetSelectedAxis(ESelectedAxis::Z);
            }
        }
    }

}

void APicker::PickObjectByRay()
{
	// 1. 마우스 커서 위치를 NDC로 변환
    POINT pt;
    GetCursorPos(&pt);

	ScreenToClient(UEngine::Get().GetWindowHandle(), &pt);
	float ScreenWidth = UEngine::Get().GetScreenWidth();
	float ScreenHeight = UEngine::Get().GetScreenHeight();

	float NDCX = 2.0f * pt.x / ScreenWidth - 1.0f;
	float NDCY = -2.0f * pt.y / ScreenHeight + 1.0f;

	// 2. Projection 공간으로 변환
	FMatrix InvProjMat = UEngine::Get().GetRenderer()->GetProjectionMatrix().Inverse();

    FVector4 NearPoint = InvProjMat.TransformVector4(NearPoint);
    FVector4 FarPoint = InvProjMat.TransformVector4(FarPoint);

	// 3. View 공간으로 변환
	FMatrix InvViewMat = FEditorManager::Get().GetCamera()->GetViewMatrix().Inverse();
	NearPoint = InvProjMat.TransformVector4(FVector4(NDCX, NDCY, 0.0f, 1.0f));
	FarPoint = InvProjMat.TransformVector4(FVector4(NDCX, NDCY, 1.0f, 1.0f));

	// 4. 월드 공간으로 변환
	NearPoint /= NearPoint.W;
    FarPoint /= FarPoint.W;

	NearPoint = InvViewMat.TransformVector4(NearPoint);
	FarPoint = InvViewMat.TransformVector4(FarPoint);

	FVector RayOrigin = FVector(NearPoint.X, NearPoint.Y, NearPoint.Z);
	FVector RayEnd = FVector(FarPoint.X, FarPoint.Y, FarPoint.Z);

	FVector RayDir = (RayEnd - RayOrigin).GetSafeNormal();

	CheckRayIntersection(RayOrigin, RayDir);

}

bool APicker::CheckRayIntersection(const FVector& RayOrigin, const FVector& RayDir)
{
	// !TODO: Ray와 오브젝트의 충돌 체크
	const TArray<USceneComponent*>& SceneComponents = UEngine::Get().GetObjectArrayByType<USceneComponent>();

    for (USceneComponent* SceneComponent : SceneComponents)
    {
        //if (SceneComponent->IsVisible() == false) continue;
        //if (SceneComponent->IsCollisionEnabled() == false) continue;
        //FTransform ComponentTransform = SceneComponent->GetComponentTransform();
        //FBox ComponentAABB = SceneComponent->GetAABB();
        //FBox ComponentAABBInWorld = ComponentAABB.TransformBy(ComponentTransform);
        //if (ComponentAABBInWorld.IntersectRay(RayOrigin, RayDir))
        //{
        //	return true;
        //}
    // UEngine의 USceneComponent 전부 가져오기


    // 각 ActorComponent의 AABB와 Ray의 충돌 체크
    // 충돌한 ActorComponent를 리턴
        return false;
    }
}
