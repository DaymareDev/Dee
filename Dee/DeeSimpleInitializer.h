#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

namespace Dee
{
	/**
	* Contains the required elements to run a directx application.
	*
	*/
	struct DeeSimpleDX
	{
		IDXGISwapChain* Swapchain;             // the pointer to the swap chain interface
		ID3D11Device* Device;                     // the pointer to our Direct3D device interface
		ID3D11DeviceContext* DeviceContext;           // the pointer to our Direct3D device context
		ID3D11RenderTargetView* Backbuffer;
	};

	/* *
		Assists the user in initializing directx11.
	*/
	class DeeSimpleInitializer
	{
	public:

		DeeSimpleInitializer();
		~DeeSimpleInitializer();

		DeeSimpleDX Initialize(short gameWindowWidth, short gameWindowHeight, HWND windowHandle);
		void CleanD3D(DeeSimpleDX* activeSimpleDx);         // closes Direct3D and releases memory

	private:
	};

};