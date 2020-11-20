#pragma once
#pragma once
#ifndef __d3dUtilityH__
#define __d3dUtilityH__

#include <windows.h>
//////////////////////////////////////////////////////
//XNA数学库相关头文件
//////////////////////////////////////////////////////
#include <d3dcompiler.h>
#include <xnamath.h>

//////////////////////////////////////////////////////
//DirectX 11相关头文件
//////////////////////////////////////////////////////
#include <d3d11.h>
#include <d3dx11.h>
#include<d3dx11effect.h>
/////////////////////////////////////////////////////
//DirectX11相关库
/////////////////////////////////////////////////////
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Effects11.lib")
namespace d3d//定义一个d3d命名空间
{//初始化D3D
	bool InitD3D(
		HINSTANCE hInstance, int width, int height, ID3D11RenderTargetView** renderTargetView, ID3D11DeviceContext** immediateContext,
		IDXGISwapChain** swapChain, ID3D11Device** device);

	//消息循环
	int EnterMsgLoop(bool(*ptr_display)(float timeDelta));

	//回调函数
	LRESULT CALLBACK WndProc(
		HWND,
		UINT msg,
		WPARAM,
		LPARAM lParam);
}
#endif