#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include "DeeSimpleInitializer.h"

auto windowWidth = 800;
auto windowHeight = 600;
Dee::DeeSimpleDX DeeDX;

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

	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;
	auto initializer = Dee::DeeSimpleInitializer();
	
	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
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
		nullptr,    // we have no parent window, NULL
		nullptr,    // we aren't using menus, NULL
		hInstance,    // application handle
		nullptr);    // used with multiple windows, NULL
	
	DeeDX = initializer.Initialize(windowWidth, windowHeight, hWnd);
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
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}
	}
	
	initializer.CleanD3D(&DeeDX);
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

void RenderFrame(void)
{
	// clear the back buffer to a deep blue
	DeeDX.DeviceContext->ClearRenderTargetView(DeeDX.Backbuffer, reinterpret_cast<float*>(&DirectX::XMFLOAT4(0.0f, 0.2f, 0.4f, 1.0f))); // reinterpret_cast<float*>(&DirectX::PackedVector::XMCOLOR(0.0f, 0.2f, 0.4f, 1.0f)

	// do 3D rendering on the back buffer here

	// switch the back buffer and the front buffer
	DeeDX.Swapchain->Present(0, 0);
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