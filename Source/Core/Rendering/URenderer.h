﻿#pragma once

#define _TCHAR_DEFINED
#include <d3d11.h>
#include <memory>

#include "UI.h"
#include "Core/Math/Vector.h"
// #include "Object/Actor/Camera.h"
#include "Core/Rendering/BufferCache.h"
#include "Core/Math/Matrix.h"
#include "Core/Engine.h"
#include "Primitive/PrimitiveVertices.h"
#include "Core/Math/Quat.h"


struct FVertexSimple;
struct FVector4;

class ACamera;
struct FBox;

class URenderer
{
private:
    struct alignas(16) FConstants
    {
        FMatrix MVP;
        FVector4 Color;
		// true인 경우 Vertex Color를 사용하고, false인 경우 Color를 사용합니다.
        uint32 bUseVertexColor;
        FVector Padding;
    };
	
	struct alignas(16) FPickingConstants
	{
		FVector4 UUIDColor;
	};

	struct alignas(16) FDepthConstants{
		unsigned int DepthOffset;
		int nearPlane;
		int farPlane;
	};
	
    struct ConstantUpdateInfo
    {
        const FMatrix& ModelMatrix;
		const FVector4& Color;
        bool bUseVertexColor;
    };

public:
    /** Renderer를 초기화 합니다. */
    void Create(HWND hWindow);

    /** Renderer에 사용된 모든 리소스를 해제합니다. */
    void Release();

    void CreateShader();

    void ReleaseShader();

    void CreateConstantBuffer();

    void ReleaseConstantBuffer();

    /** 스왑 체인의 백 버퍼와 프론트 버퍼를 교체하여 화면에 출력 */
    void SwapBuffer() const;

    /** 렌더링 파이프라인을 준비 합니다. */
    void Prepare() const;

    /** 셰이더를 준비 합니다. */
    void PrepareShader() const;

	void RenderPrimitive(class UPrimitiveComponent* PrimitiveComp);

    /**
     * Buffer에 있는 Vertex를 그립니다.
     * @param pBuffer 렌더링에 사용할 버텍스 버퍼에 대한 포인터
     * @param numVertices 버텍스 버퍼에 저장된 버텍스의 총 개수
     */
    void RenderPrimitiveInternal(ID3D11Buffer* pBuffer, UINT numVertices) const;

	void RenderBox(const FBox& Box, const FVector4& Color = FVector4(1,1,1,1)) const;

    /**
     * 정점 데이터로 Vertex Buffer를 생성합니다.
     * @param Vertices 버퍼로 변환할 정점 데이터 배열의 포인터
     * @param ByteWidth 버퍼의 총 크기 (바이트 단위)
     * @return 생성된 버텍스 버퍼에 대한 ID3D11Buffer 포인터, 실패 시 nullptr
     *
     * @note 이 함수는 D3D11_USAGE_IMMUTABLE 사용법으로 버퍼를 생성합니다.
     */
    ID3D11Buffer* CreateVertexBuffer(const FVertexSimple* Vertices, UINT ByteWidth) const;

    /** Buffer를 해제합니다. */
    void ReleaseVertexBuffer(ID3D11Buffer* pBuffer) const;

    /** Constant Data를 업데이트 합니다. */
    void UpdateConstant(const ConstantUpdateInfo& UpdateInfo) const;

    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetDeviceContext() const;

    /** View 변환 Matrix를 업데이트 합니다. */
    void UpdateViewMatrix(const FTransform& CameraTransform);

    /** Projection 변환 Matrix를 업데이트 합니다. */
    void UpdateProjectionMatrix(ACamera* Camera);

	void OnUpdateWindowSize(int Width, int Height);

	void GetPrimitiveLocalBounds(EPrimitiveType Type, FVector& OutMin, FVector& OutMax);

protected:
    /** Direct3D Device 및 SwapChain을 생성합니다. */
    void CreateDeviceAndSwapChain(HWND hWindow);

    /** Direct3D Device 및 SwapChain을 해제합니다.  */
    void ReleaseDeviceAndSwapChain();

    /** 프레임 버퍼를 생성합니다. */
    void CreateFrameBuffer();

    /** 뎁스 스텐실 버퍼를 생성합니다. */
	void CreateDepthStencilBuffer();

    /** 뎁스 스텐실 상태를 생성합니다. */
	void CreateDepthStencilState();
	
    /** 프레임 버퍼를 해제합니다. */
    void ReleaseFrameBuffer();

	/** 뎁스 스텐실 버퍼를 해제합니다. */
	void ReleaseDepthStencilBuffer();
	
    /** 레스터라이즈 상태를 생성합니다. */
    void CreateRasterizerState();

    /** 레스터라이저 상태를 해제합니다. */
    void ReleaseRasterizerState();

    void CreateBufferCache();

    void InitMatrix();

protected:
    // Direct3D 11 장치(Device)와 장치 컨텍스트(Device Context) 및 스왑 체인(Swap Chain)을 관리하기 위한 포인터들
    ID3D11Device* Device = nullptr;                         // GPU와 통신하기 위한 Direct3D 장치
    ID3D11DeviceContext* DeviceContext = nullptr;           // GPU 명령 실행을 담당하는 컨텍스트
    IDXGISwapChain* SwapChain = nullptr;                    // 프레임 버퍼를 교체하는 데 사용되는 스왑 체인

    // 렌더링에 필요한 리소스 및 상태를 관리하기 위한 변수들
    ID3D11Texture2D* FrameBuffer = nullptr;                 // 화면 출력용 텍스처
    ID3D11RenderTargetView* FrameBufferRTV = nullptr;       // 텍스처를 렌더 타겟으로 사용하는 뷰
    ID3D11RasterizerState* RasterizerState = nullptr;       // 래스터라이저 상태(컬링, 채우기 모드 등 정의)
    ID3D11Buffer* ConstantBuffer = nullptr;                 // 쉐이더에 데이터를 전달하기 위한 상수 버퍼

    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; // 화면을 초기화(clear)할 때 사용할 색상 (RGBA)
    D3D11_VIEWPORT ViewportInfo = {};                       // 렌더링 영역을 정의하는 뷰포트 정보

    // Shader를 렌더링할 때 사용되는 변수들
    ID3D11VertexShader* SimpleVertexShader = nullptr;       // Vertex 데이터를 처리하는 Vertex 셰이더
    ID3D11PixelShader* SimplePixelShader = nullptr;         // Pixel의 색상을 결정하는 Pixel 셰이더

    ID3D11InputLayout* SimpleInputLayout = nullptr;         // Vertex 셰이더 입력 레이아웃 정의
    unsigned int Stride = 0;                                // Vertex 버퍼의 각 요소 크기

    // Depth Stenil Buffer
	ID3D11Texture2D* DepthStencilBuffer = nullptr;          // DepthStencil버퍼 역할을 하는 텍스쳐
	ID3D11DepthStencilView* DepthStencilView = nullptr;     // DepthStencil버퍼를 렌더 타겟으로 사용하는 뷰
	ID3D11DepthStencilState* DepthStencilState = nullptr;   // DepthStencil 상태(깊이 테스트, 스텐실 테스트 등 정의)
    ID3D11DepthStencilState* GizmoDepthStencilState = nullptr; // 기즈모용 스텐실 스테이트. Z버퍼 테스트 하지않고 항상 앞에렌더
	
	// Buffer Cache

	std::unique_ptr<class FBufferCache> BufferCache;

	FMatrix WorldMatrix;
    FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;

	D3D_PRIMITIVE_TOPOLOGY CurrentTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;

	
#pragma region picking
protected:
	// 피킹용 버퍼들
	ID3D11Texture2D* PickingFrameBuffer = nullptr;                 // 화면 출력용 텍스처
	ID3D11RenderTargetView* PickingFrameBufferRTV = nullptr;       // 텍스처를 렌더 타겟으로 사용하는 뷰
	ID3D11Buffer* ConstantPickingBuffer = nullptr;                 // 뷰 상수 버퍼
	FLOAT PickingClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //
	ID3D11PixelShader* PickingPixelShader = nullptr;         // Pixel의 색상을 결정하는 Pixel 셰이더
	ID3D11Buffer* ConstantsDepthBuffer = nullptr;

	ID3D11DepthStencilState* IgnoreDepthStencilState = nullptr;   // DepthStencil 상태(깊이 테스트, 스텐실 테스트 등 정의)

public:
	//피킹용 함수들	
    void ReleasePickingFrameBuffer();
    void CreatePickingTexture(HWND hWnd);
    void PrepareZIgnore() const;
    void PreparePicking() const;
	void PreparePickingShader() const;
	void UpdateConstantPicking(FVector4 UUIDColor) const;
    void UpdateConstantDepth(int Depth) const;

    void PrepareMain();
	void PrepareMainShader();

	FVector4 GetPixel(FVector MPos);

	void RenderPickingTexture();
	FMatrix GetProjectionMatrix() const { return ProjectionMatrix; }
#pragma endregion picking
};
