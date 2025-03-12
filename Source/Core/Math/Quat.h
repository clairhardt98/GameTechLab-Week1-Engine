#pragma once
#include "Vector.h"

#define PIDIV2 3.141592654/2

struct alignas(16) FQuaternion : public FVector4{
    using FVector::X;
    using FVector::Y;
    using FVector::Z;
    using FVector4::W;

    FQuaternion() : FVector4(0.0f, 0.0f, 0.0f, 1.0f) {}
	explicit FQuaternion(float InX, float InY, float InZ, float InW) : FVector4(InX, InY, InZ, InW) {}
    explicit FQuaternion(FVector Rotation) : FVector4(EulerToQuaternion(Rotation)) {}
    FQuaternion(const FVector& Axis, float AngleInDegrees, bool bIsDegree) : FVector4(AxisAngleToQuaternion(Axis, AngleInDegrees, bIsDegree)) {}
    FQuaternion(const FQuaternion& Quat);

    static FQuaternion EulerToQuaternion(const FVector& Euler);
    static FVector QuaternionToEuler(const FQuaternion& quaternion);
    static FQuaternion AxisAngleToQuaternion(const FVector& Axis, float AngleInDegrees, bool bIsDegree);

    static FQuaternion AddQuaternions(const FQuaternion& q1, const FQuaternion& q2);
    static FQuaternion MultiplyQuaternions(const FQuaternion& q1, const FQuaternion& q2);
	FQuaternion operator*(const FQuaternion& Other) const { return MultiplyQuaternions(*this, Other); }
    static FQuaternion SubtractQuaternions(const FQuaternion& q1, const FQuaternion& q2);

    static FQuaternion MakeFromRotationMatrix(const struct FMatrix& M);
	FVector RotateVector(const FVector& Other) const;
    FVector GetEuler() const { return QuaternionToEuler(*this); }
    void Normalize();
    bool IsNormalized() const;
};
