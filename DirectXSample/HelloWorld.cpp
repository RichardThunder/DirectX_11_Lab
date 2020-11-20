#if 1
#include<Windows.h>
#include <wrl/client.h>
HWND MainWindowHandle = 0;

bool InitWindowsApp(HINSTANCE instanceHandle, int show);

int Run();

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	if (!InitWindowsApp(hInstance, nShowCmd))
	{
		::MessageBox(0, L"Init-Failed", L"error", MB_OK);
		return 0;
	}
	// 允许在Debug版本进行运行时内存分配和泄漏检测
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	return Run();
}


bool InitWindowsApp(HINSTANCE instanceHandle, int show)//���ڳ�ʼ������
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = ::LoadIcon(0, IDI_WARNING);
	wc.hCursor = ::LoadCursor(0, IDC_PERSON);
	wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(GRAY_BRUSH));
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Hello";

	if (!::RegisterClass(&wc))
	{
		::MessageBox(0, L"RegisterClass -Filed", 0, 0);
		return false;
	}


	MainWindowHandle = ::CreateWindow(L"Hello", L"Hello",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, instanceHandle, 0);

	if (MainWindowHandle == 0)
	{
		::MessageBox(0, L"CreateWindow -Failed", 0, 0);
		return false;
	}

	::ShowWindow(MainWindowHandle, show);
	::UpdateWindow(MainWindowHandle);
	return true;
}

int Run()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	while (::GetMessage(&msg, 0, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		::MessageBox(0, L"ERROR", L"hello,ERROR", MB_OK);
		return 0;
	case WM_RBUTTONDOWN:
		::MessageBox(0, L"EEEEEEEEEEEEEEERROR", L"ERROR", MB_OK);
		return 0;
	case VK_ESCAPE:
		::DestroyWindow(MainWindowHandle);
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(MainWindowHandle);
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return::DefWindowProc(windowHandle, msg, wParam, lParam);
}

#endif
