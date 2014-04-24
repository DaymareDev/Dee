#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>


// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;

// function prototypes
void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
void CleanD3D(void);         // closes Direct3D and releases memory
void RenderFrame();

LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	auto windowWidth = 800;
	auto windowHeight = 600;
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "DeeCore";

	// register the window class
	RegisterClassEx(&wc);
	RECT adjustedWindowArea = { 0, 0, windowWidth, windowHeight };
	AdjustWindowRect(&adjustedWindowArea, WS_OVERLAPPEDWINDOW, false);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		"DeeCore",    // name of the window class
		"Dee Debug Window",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		adjustedWindowArea.right - adjustedWindowArea.left,    // width of the window
		adjustedWindowArea.bottom - adjustedWindowArea.top,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL
	
	InitD3D(hWnd);
	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// wait for the next message in the queue, store the result in 'msg'
	while (msg.message != WM_QUIT)
	{
		RenderFrame();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}
	}
	
	CleanD3D();
	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	

	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();
	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	devcon->RSSetViewports(1, &viewport);
}

void RenderFrame(void)
{
	// clear the back buffer to a deep blue
	devcon->ClearRenderTargetView(backbuffer, reinterpret_cast<float*>(&DirectX::XMFLOAT4(0.0f, 0.2f, 0.4f, 1.0f))); // reinterpret_cast<float*>(&DirectX::PackedVector::XMCOLOR(0.0f, 0.2f, 0.4f, 1.0f)

	// do 3D rendering on the back buffer here

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}

void CleanD3D()
{
	// close and release all existing COM objects
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}


/**
Multimonitor stuff
auto monitorPtr = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);
LPMONITORINFO monitorInfo = 0;
if (GetMonitorInfo(monitorPtr, monitorInfo))
{
auto monitorArea = monitorInfo->rcMonitor;
auto monitorWidth = monitorArea.right - monitorArea.left;
auto monitorHeight = monitorArea.bottom - monitorArea.top;
auto success = SetWindowPos(hWnd, NULL,
monitorArea.left + (monitorWidth - windowWidth) / 2,
monitorArea.top + (monitorHeight - windowHeight) / 2,
monitorWidth, monitorHeight, SWP_SHOWWINDOW);
if (success != true)
throw "fail";
}

*/