#pragma once
#include <memory>
#include "Core/HAL/PlatformType.h"


class UObject : public std::enable_shared_from_this<UObject>
{
	friend class FObjectFactory;

	uint32 UUID = 0;
	uint32 InternalIndex; // Index of GUObjectArray

public:
	UObject();
	virtual ~UObject();

public:
	uint32 GetUUID() const { return UUID; }
	uint32 GetInternalIndex() const { return InternalIndex; }
};