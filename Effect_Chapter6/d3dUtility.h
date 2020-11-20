#pragma once
#pragma once
#ifndef __d3dUtilityH__
#define __d3dUtilityH__

#include <windows.h>
//////////////////////////////////////////////////////
//XNA��ѧ�����ͷ�ļ�
//////////////////////////////////////////////////////
#include <d3dcompiler.h>
#include <xnamath.h>

//////////////////////////////////////////////////////
//DirectX 11���ͷ�ļ�
//////////////////////////////////////////////////////
#include <d3d11.h>
#include <d3dx11.h>
#include<d3dx11effect.h>
/////////////////////////////////////////////////////
//DirectX11��ؿ�
/////////////////////////////////////////////////////
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Effects11.lib")
namespace d3d//����һ��d3d�����ռ�
{//��ʼ��D3D
	bool InitD3D(
		HINSTANCE hInstance, int width, int height, ID3D11RenderTargetView** renderTargetView, ID3D11DeviceContext** immediateContext,
		IDXGISwapChain** swapChain, ID3D11Device** device);

	//��Ϣѭ��
	int EnterMsgLoop(bool(*ptr_display)(float timeDelta));

	//�ص�����
	LRESULT CALLBACK WndProc(
		HWND,
		UINT msg,
		WPARAM,
		LPARAM lParam);
}
#endif