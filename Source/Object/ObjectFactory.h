#pragma once
#include "Core/Engine.h"
#include "Core/EngineStatics.h"
#include "Core/HAL/PlatformMemory.h"
#include "Debug/DebugConsole.h"

class UObject;

class FObjectFactory
{
public:
    /**
     * UObject를 생성합니다.
     * @tparam T UObject를 상속받은 클래스
     * @return 캐스팅된 UObject*
     */
    template<typename T>
        requires std::derived_from<T, UObject>
    static T* ConstructObject()
    {
        UE_LOG("DEBUG: Construct %s Object", typeid(T).name());

        constexpr size_t ObjectSize = sizeof(T);
        void* RawMemory = FPlatformMemory::Malloc<EAT_Object>(ObjectSize);

        T* ObjectPtr = new (RawMemory) T();
        std::shared_ptr<T> NewObject(ObjectPtr, [ObjectSize](T* Obj)
        {
            Obj->~T();
            FPlatformMemory::Free<EAT_Object>(Obj, ObjectSize);
        });
        NewObject->UUID = UEngineStatics::GenUUID();
        UEngine::Get().GObjects.Add(NewObject->GetUUID(), NewObject);

        return NewObject.get();
    }
};
