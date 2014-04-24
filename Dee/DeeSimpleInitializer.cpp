#include "DeeSimpleInitializer.h"

using namespace Dee;

DeeSimpleInitializer::DeeSimpleInitializer()
{
}

DeeSimpleInitializer::~DeeSimpleInitializer()
{
}

DeeSimpleDX Dee::DeeSimpleInitializer::Initialize(short gameWindowWidth, short gameWindowHeight, HWND windowHandle)
{
	DeeSimpleDX dx;
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&dx, sizeof(DeeSimpleDX));
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = windowHandle;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		NULL,
		nullptr,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&dx.Swapchain,
		&dx.Device,
		nullptr,
		&dx.DeviceContext);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	dx.Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));

	// use the back buffer address to create the render target
	dx.Device->CreateRenderTargetView(pBackBuffer, nullptr, &dx.Backbuffer);
	pBackBuffer->Release();
	// set the render target as the back buffer
	dx.DeviceContext->OMSetRenderTargets(1, &dx.Backbuffer, nullptr);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = gameWindowWidth;
	viewport.Height = gameWindowWidth;

	dx.DeviceContext->RSSetViewports(1, &viewport);
	return dx;
}

void DeeSimpleInitializer::CleanD3D(DeeSimpleDX* activeSimpleDx)
{
	// close and release all existing COM objects
	activeSimpleDx->Swapchain->SetFullscreenState(FALSE, nullptr);
	activeSimpleDx->Swapchain->Release();
	activeSimpleDx->Backbuffer->Release();
	activeSimpleDx->DeviceContext->Release();
	activeSimpleDx->Device->Release();
}
