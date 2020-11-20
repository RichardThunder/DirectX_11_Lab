#include"d3dUtility.h"
ID3D11Device* device = NULL;//D3 D11设备指针
IDXGISwapChain* swapChain = NULL;//交换链指针
ID3D11DeviceContext* immediateContext = NULL;//执行上下文指针
ID3D11RenderTargetView* renderTargetView = NULL;//渲染目标视图指针
//**************　以下为框架函数******************
bool Setup()
{//本例中setup函数没有内容，以后的实验中会往里面填写内容
	return true;
}
void Cleanup()
{
	//释放指针
	if (renderTargetView) renderTargetView->Release();
	if (immediateContext) immediateContext->Release();
	if (swapChain) swapChain->Release();
	if (device)device->Release();
}
bool Display(float timeDelta)
{
	if (device)
	{//声明一个数组存放颜色信息，4个元素分别表示红，绿，蓝以及alpha
		float ClearColor[4] = { 0.0f,0.125f,0.3f,1.0f };
		//清除渲染目标视图
		immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
		//显示渲染好的图像给用户
		swapChain->Present(0,//指定如何同步显示，设置0表示不同步显示
			0);//可选项，设置0表示为从每个缓存中显示一帧
	}
	return true;
}
//**************　框架函数编写结束*****************

///////////////////////////////////////////////
//回调函数
///////////////////////////////////////////////
LRESULT CALLBACK d3d::WndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return::DefWindowProc(hwnd, msg, wParam, lParam);
}

/*主函数
WinMain函数展现出了D3 D程序设计的主程序结构。
①调用InitD3 D函数初始化；
②调用Setup函数建模；
③进入消息循环函数EnterMsgLoop；
④释放资源Cleanup。
这种结构的优点在于只要程序处于空闲状态，则会调用渲染函数，运行效率较高。*/
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	//初始化
	//**注意**代码4.8声明的4个指针，在这里作为参数传给InitD3 D函数
	if (!d3d::InitD3D(hinstance, 800, 600, &renderTargetView, &immediateContext, &swapChain, &device))
	{
		::MessageBox(0, L"Initd3d() -Failed", 0, 0);
		return 0;
	}
	if (!Setup())
	{
		::MessageBox(0, L"Steup() -Failed", 0, 0);
		return 0;
	}
	//执行消息循环，将函数Display的指针作为参数传递
	d3d::EnterMsgLoop(Display);
	Cleanup();
	return 0;
}