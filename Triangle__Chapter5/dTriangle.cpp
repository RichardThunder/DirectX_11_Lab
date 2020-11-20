#include "d3dUtility.h"
ID3D11Device *device = NULL;					 //D3 D11�豸ָ��
IDXGISwapChain *swapChain = NULL;				 //������ָ��
ID3D11DeviceContext *immediateContext = NULL;	 //ִ��������ָ��
ID3D11RenderTargetView *renderTargetView = NULL; //��ȾĿ����ͼָ��
ID3D11VertexShader *vertexShader;				 //������ɫ��
ID3D11PixelShader *pixelShader;					 //������ɫ��
//����һ������ṹ���������ֻ��Ҫ����������Ϣ
struct Vertex
{
	XMFLOAT3 Pos;
};
//**************������Ϊ��ܺ���******************
bool Setup()
{ //������setup����û�����ݣ��Ժ��ʵ���л���������д����
	/*��һ��������������ɫ����
		�ڶ���������������ɫ����
		���������������������벼�֣�
		���Ĳ����������㻺�档*/
	//��һ��������������ɫ��
	//����һ����ɫ�����ʶ��
	//###################D3 DCOMPILE_ENABLE_STRICTNESS��ʾǿ���ϸ����###################
	//����һЩ�Ѿ��������﷨�����ܱ���
	DWORD dwShaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;
	//ID3 DBlob�ӿ����ڱ�ʾ���ⳤ�ȵ�����
	//����ʹ�øýӿ�����һ����������ű�������ɫ��
	ID3DBlob *pVSBlob = NULL;
	//��ָ���ļ�������ɫ��
	if (FAILED(D3DX11CompileFromFile(
			L"Triangle.hlsl", //�������Ǵ�����hlsl�ļ�
			NULL,			  //��ѡ�ָ��һ���궨�������ָ�룬һ��ΪNULL
			NULL,			  //���ڴ�����ɫ���е�include�ļ���û��include�ļ���ΪNULL
			"VSMain",		  //ָ��������ɫ������ں���
			"vs_5_0",		  //ָ��������ɫ���汾����������5.0�汾
			dwShaderFlag,	  //���涨�����ɫ�����ʶ��
			0,				  //Effect�����ʶ����������Ǳ���Effect�ļ�������Ϊ0
			NULL,			  //ָ��ID3 DX11 ThreadPump��ָ�룬
			//����ΪNULL��ʾ������������֮ǰ���᷵��
			&pVSBlob, //ָ�����õĶ�����ɫ���������ڴ��ָ��
			NULL,	  //ָ�����ڱ����в�������;����ָ��
			NULL	  // һ��ָ�򷵻�ֵ��ָ�룬����ڰ˸�����ΪNULL
			)))		  //����Ҳ����ΪNULL
	{
		//�������ʧ�ܣ�����һ����Ϣ��
		MessageBox(NULL, L"fail to compile vertex shader", L"Error", MB_OK);
		return S_FALSE;
	}
	//��CreateVertexShader����������ɫ��
	device->CreateVertexShader(
		pVSBlob->GetBufferPointer(), //ָ��pVSBlob�����ڴ�����ʼ��ַ
		pVSBlob->GetBufferSize(),	 //ȡ��pVSBlob�Ĵ�С
		NULL,						 //ָ��ID3 D11 ClassLinkage��ָ�룬һ��Ϊ��
		&vertexShader				 //�������õĶ�����ɫ����ŵ�vertexShader��
	);
	//�ڶ���������������ɫ��
	//����һ��ID3 DBlob��������ű�����������ɫ��
	ID3DBlob *pPSBlob = NULL;
	if (FAILED(D3DX11CompileFromFile(
			L"Triangle.hlsl",
			NULL,
			NULL,
			"PSMain",
			"ps_5_0",
			dwShaderFlag,
			0,
			NULL,
			&pPSBlob,
			NULL,
			NULL)))
	{
		MessageBox(NULL, L"Failed to compile pixel shader", L"Error", MB_OK);
		return S_FALSE;
	}
	//��CreatePixelShader����������ɫ��
	device->CreatePixelShader(
		pPSBlob->GetBufferPointer(),
		pPSBlob->GetBufferSize(),
		NULL,
		&pixelShader); //�������õĶ�����ɫ����ŵ�pixelShader��
	//���������������������벼��
	//��ν���벼�֣�����ָ������ṹ��������Ϣ��ʵ������
	//D3 D11_INPUT_ELEMENT_DESC������������ṹ������
	//�Ա����Կ�ʶ��������Ķ���ṹ
	//���ﶨ��һ��D3 D11_INPUT_ELEMENT_DESC���飬
	//���ڶ���Ķ���ṹֻ��λ�����꣬�������������Ҳֻ��һ��Ԫ��
	D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{
				"POSITION",						//�����ʶ���������hlsl�ļ���VSMain���õı�ʶ��һ��
				0,								//��ʾ��1��Ԫ�أ�ע�������һ�����Ǵ�0��ʼ������
				DXGI_FORMAT_R32G32B32_FLOAT, //96λ�������أ���������32λ
				0,								//����ȡֵ0-15��0��ʾ�󶨶��㻺�浽��һ�������
				0,								//��ѡ������˻���Ķ��뷽ʽ��
				D3D11_INPUT_PER_VERTEX_DATA,	//����������������Ϊ��������
				0								//����û��ʹ��ʵ����������������Ϊ0
			}
		};
	//��ȡ���벼����Ԫ�ظ���
	UINT numElements = ARRAYSIZE(layout);
	//����һ�����벼�ֶ���pVertexLayout���ڴ�Ŵ����õĲ���
	ID3D11InputLayout *pVertexLayout;
	//����CreateInputLayout�������벼��
	device->CreateInputLayout(
		layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&pVertexLayout);
	//�������ɵ����벼��
	immediateContext->IASetInputLayout(pVertexLayout);
	//���Ĳ����������㻺��
	//�������Լ������Vertex�ṹ���������ε�������������
	Vertex vertices[] =
		{
			XMFLOAT3(-0.5f, 0.0f, 0.0f),
			XMFLOAT3(0.0f, 0.5f, 0.0f),
			XMFLOAT3(0.5f, 0.0f, 0.0f),
			XMFLOAT3(-0.5f, 0.0f, 0.0f),
			XMFLOAT3(0.3f, 0.0f, 0.0f),
			XMFLOAT3(0.0f,-0.6f, 0.0f),
		};
	//���D3 D11_BUFFER_DESC�ṹ������ṹ�������������㻺�������
	//��������һ��D3 D11_BUFFER_DESC�Ķ���bd
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));//�����������
	bd.Usage = D3D11_USAGE_DEFAULT;//���û���Ķ�д��ʽ��һ����Ĭ�Ϸ�
	bd.ByteWidth = sizeof(Vertex) * 6;//���û�������Ĵ�С//������������������Ҫ����3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//�������������󶨵����㻺��
	bd.CPUAccessFlags = 0;//CPU���ʱ�ʶ����0��ʾû��CPU����
	bd.MiscFlags = 0;//�������ʶ����0��ʾ��ʹ�ø���

	//����һ���������ڳ�ʼ������Դ
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));//�����������
	InitData.pSysMem = vertices;//������Ҫ��ʼ�������ݣ�����������
	//����һ��ID3 D11 Buffer������Ϊ���㻺��
	ID3D11Buffer* vertexBuffer;
	//����CreateBuffer��������
	device->CreateBuffer(&bd, &InitData, &vertexBuffer);
	UINT stride = sizeof(Vertex);//��ȡVertex�Ĵ�С��Ϊ���
	UINT offset = 0;//����ƫ����Ϊ0
	//���ö��㻺��
	immediateContext->IASetVertexBuffers(
		0,//�󶨵���һ�������
		1,//���㻺��ĸ���������Ϊһ��
		&vertexBuffer,//�����õĶ��㻺��
		&stride,//��ȣ�������ṹ�Ĵ�С
		&offset//�����һ��Ԫ�ص�����Ԫ�ص�ƫ����
	);
	//ָ��ͼԪ���ͣ�D3 D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST��ʾͼԪΪ������
	immediateContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}
void Cleanup()
{
	//�ͷ�ָ��
	if (renderTargetView)
		renderTargetView->Release();
	if (immediateContext)
		immediateContext->Release();
	if (swapChain)
		swapChain->Release();
	if (swapChain)
		device->Release();
	if (vertexShader)
		vertexShader->Release();
	if (pixelShader)
		pixelShader->Release();
}
bool Display(float timeDelta)
{
	if (device)
	{ //����һ����������ɫ��Ϣ��4��Ԫ�طֱ��ʾ�죬�̣����Լ�alpha
		float ClearColor[4] = {0.0f, 0.125f, 0.3f, 1.0f};
		//�����ȾĿ����ͼ
		immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
		//�Ѵ����õĶ�����ɫ����������ɫ���󶨵�immediateContext
		immediateContext->VSSetShader(vertexShader, NULL, 0);
		immediateContext->PSSetShader(pixelShader, NULL, 0);
		//���������Σ���һ��������ʾ����3���㣬
		//�ڶ���������ʾ�ӵ�0���㿪ʼ����
		immediateContext->Draw(6, 0);
		//��ʾ��Ⱦ�õ�ͼ����û�
		swapChain->Present(0,  //ָ�����ͬ����ʾ������0��ʾ��ͬ����ʾ
						   0); //��ѡ�����0��ʾΪ��ÿ����������ʾһ֡
	}
	return true;
}
//**************����ܺ�����д����*****************

///////////////////////////////////////////////
//�ص�����
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
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

/*������
WinMain����չ�ֳ���D3 D������Ƶ�������ṹ��
�ٵ���InitD3 D������ʼ����
�ڵ���Setup������ģ��
�۽�����Ϣѭ������EnterMsgLoop��
���ͷ���ԴCleanup��
���ֽṹ���ŵ�����ֻҪ�����ڿ���״̬����������Ⱦ����������Ч�ʽϸߡ�*/
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance,
				   PSTR cmdLine,
				   int showCmd)
{
	//��ʼ��
	//**ע��**����4.8������4��ָ�룬��������Ϊ��������InitD3 D����
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
	//ִ����Ϣѭ����������Display��ָ����Ϊ��������
	d3d::EnterMsgLoop(Display);
	Cleanup();
}


/*************************************************************************
    > File Name: d3dTriangle.cpp
    > Author: YLD10
    > Mail: yl1315348050@yahoo.com
    > Created Time: 2018.6.1 21:35
    > Describe: Win32 Application InitD3D.
                ������ DirectX11 �� 3D ͼ�γ�����ư����̡̳�
                5.2 �ڳ������� ����һ��������
 ************************************************************************/
//#include "d3dUtility.h"
//
//ID3D11Device* device = NULL;    // D3D11 �豸ָ��
//IDXGISwapChain* swapChain = NULL;    // ������ָ��
//ID3D11DeviceContext* immediateContext = NULL;    // ִ��������ָ��
//ID3D11RenderTargetView* renderTargetView = NULL;    // ��ȾĿ����ͼָ��
//// ����������ȫ�ֱ���
//ID3D11VertexShader* vertexShader;               // ������ɫ��
//ID3D11PixelShader* pixelShader;                // ������ɫ��
//
//// ����һ������ṹ, �������ֻ��Ҫ����������Ϣ
//struct Vertex
//{
//    XMFLOAT3 Pos;
//};
//
////************* ����Ϊ��ܺ��� ***************
///*
// * ��һ��, ����������ɫ��
// * �ڶ���, ����������ɫ��
// * ������, �������������벼��
// * ���Ĳ�, �������㻺��
// */
//bool Setup()
//{
//    // ��һ��, ����������ɫ��
//    // ����һ����ɫ�������ʶ��
//    // D3DCOMPILE_ENABLE_STRICTINESS ��ʾǿ���ϸ����
//    // ����һЩ�Ѿ��������﷨�����ܱ���
//    DWORD dwShaderFlag = D3DCOMPILE_ENABLE_STRICTNESS;
//
//    // ID3DBlob �ӿ����ڱ�ʾ���ⳤ�ȵ�����
//    // ����ʹ�øýӿ�����һ����������ű�������ɫ��
//    ID3DBlob* pVSBlob = NULL;
//
//    // ��ָ���ļ�������ɫ��
//    if (FAILED(D3DX11CompileFromFile(
//        L"Triangle.hlsl",  // �������Ǵ����� hlsl �ļ�
//        NULL,              // ��ѡ��, ָ��һ���궨�������ָ��, һ��Ϊ NULL
//        NULL,              // ���ڴ�����ɫ���е� include �ļ�, û�� include �ļ���Ϊ NULL
//        "VSMain",          // ָ��������ɫ������ں���
//        "vs_5_0",          // ָ��������ɫ���汾, �������� 5.0 �汾
//        dwShaderFlag,      // ���涨�����ɫ�������ʶ��
//        0,                 // Effect �����ʶ��, ������Ǳ��� Effect �ļ�������Ϊ 0
//        NULL,              // ָ�� ID3DX11ThreadPump ��ָ��,
//                           // ����Ϊ NULL ��ʾ������������֮ǰ���᷵��
//        &pVSBlob,          // ָ�����õĶ�����ɫ���������ڴ��ָ��
//        NULL,              // ָ�����ڱ����в�������;����ָ��
//        NULL)))            // һ��ָ�򷵻�ֵ��ָ��, ����ڰ˸�����Ϊ NULL,
//                           // ����Ҳ����Ϊ NULL
//    {
//        // �������ʧ��, ����һ����Ϣ��
//        MessageBox(NULL, L"Fail to compile vertex shader", L"ERROR", MB_OK);
//
//        return S_FALSE;
//    }
//
//    // �� CreateVertexShader ����������ɫ��
//    device->CreateVertexShader(
//        pVSBlob->GetBufferPointer(),  // ָ�� pVSBlob �����ڴ�����ʼ��ַ
//        pVSBlob->GetBufferSize(),     // ȡ�� pVSBlob �Ĵ�С
//        NULL,                         // ָ�� ID3D11ClassLinkage ��ָ��, һ��Ϊ��
//        &vertexShader);               // �������õĶ�����ɫ����ŵ� vertexShader ��
//
//// �ڶ���, ����������ɫ��
//// ����һ�� ID3DBlob ��������ű�����������ɫ��
//    ID3DBlob* pPSBlob = NULL;
//    if (FAILED(D3DX11CompileFromFile(
//        L"Triangle.hlsl",  // �������Ǵ����� hlsl �ļ�
//        NULL,              // ��ѡ��, ָ��һ���궨�������ָ��, һ��Ϊ NULL
//        NULL,              // ���ڴ�����ɫ���е� include �ļ�, û�� include �ļ���Ϊ NULL
//        "PSMain",          // ָ��������ɫ������ں���
//        "ps_5_0",          // ָ��������ɫ���汾, �������� 5.0 �汾
//        dwShaderFlag,      // ���涨�����ɫ�������ʶ��
//        0,                 // Effect �����ʶ��, ������Ǳ��� Effect �ļ�������Ϊ 0
//        NULL,              // ָ�� ID3DX11ThreadPump ��ָ��,
//                           // ����Ϊ NULL ��ʾ������������֮ǰ���᷵��
//        &pPSBlob,          // ָ�����õ�������ɫ���������ڴ��ָ��
//        NULL,              // ָ�����ڱ����в�������;����ָ��
//        NULL)))            // һ��ָ�򷵻�ֵ��ָ��, ����ڰ˸�����Ϊ NULL,
//                           // ����Ҳ����Ϊ NULL
//    {
//        MessageBox(NULL, L"Fail to compile pixel shader", L"ERROR", MB_OK);
//
//        return S_FALSE;
//    }
//
//    // �� CreatePixelShader ����������ɫ��
//    device->CreatePixelShader(
//        pPSBlob->GetBufferPointer(),  // ָ�� pPSBlob �����ڴ�����ʼ��ַ
//        pPSBlob->GetBufferSize(),     // ȡ�� pPSBlob �Ĵ�С
//        NULL,                         // ָ�� ID3D11ClassLinkage ��ָ��, һ��Ϊ��
//        &pixelShader);                // �������õ�������ɫ����ŵ� pixelShader ��
//
//// ������, �������������벼��
//// ��ν���벼��, �����ƶ�����ṹ��������Ϣ��ʵ������
//// D3D11_INPUT_ELEMENT_DESC ������������ṹ������
//// �Ա����Կ�ʶ��������Ķ���ṹ
//// ���ﶨ��һ�� D3D11_INPUT_ELEMENT_DESC ����,
//// ���ڶ���Ķ���ṹֻ��λ������, �������������Ҳֻ��һ��Ԫ��
//    D3D11_INPUT_ELEMENT_DESC layout[] =
//    {
//        {
//            "POSITION",  // �����ʶ��, ����� hlsl �ļ��� VSMain ���õı�ʶ��һ��
//            0,           // ��ʾ�� 1 ��Ԫ��, ע�������һ�����Ǵ� 0 ��ʼ����,
//            DXGI_FORMAT_R32G32B32_FLOAT,  // 96 λ��������, �������� 32 λ
//            0,           // ����ȡֵ 0 - 15, 0 ��ʾ�󶨶��㻺�浽��һ�������
//            0,           // ��ѡ��, �����˻���Ķ��뷽ʽ,
//            D3D11_INPUT_PER_VERTEX_DATA,     // ����������������Ϊ��������
//            0            // ����û��ʹ��ʵ������, ��������Ϊ 0
//        }
//    };
//
//    // ��ȡ���벼����Ԫ�ظ���
//    UINT numElements = ARRAYSIZE(layout);
//
//    // ����һ�����벼�ֶ��� pVertexLayout ���ڴ�Ŵ����õĲ���
//    ID3D11InputLayout* pVertexLayout;
//
//    // ���� CreateInputLayout �������벼��
//    device->CreateInputLayout(
//        layout,                     // ���涨��� D3D11_INPUT_ELEMENT_DESC ����
//        numElements,                // D3D11_INPUT_ElEMENT_DESC �����Ԫ�ظ���
//        pVSBlob->GetBufferPointer(),  // ָ�򶥵���ɫ����ʼλ�õ�ָ��
//        pVSBlob->GetBufferSize(),     // ָ�򶥵���ɫ���������ڴ��С
//        &pVertexLayout);            // �������ɵ����벼�ֶ���
//
//// �������ɵ����벼��
//    immediateContext->IASetInputLayout(pVertexLayout);
//
//    // ���Ĳ�, �������㻺��
//    // �������Լ������ Vertex �ṹ���������ε�������������
//    Vertex vertices[] =
//    {
//      XMFLOAT3(0.0f, 0.5f, 0.0f),
//      XMFLOAT3(0.5f, 0.0f, 0.0f),
//      XMFLOAT3(-0.5f, 0.0f, 0.0f)
//    };
//
//    // ��� D3D11_BUFFER_DESC �ṹ, ����ṹ�������������㻺�������
//    // ��������һ�� D3D11_BUFFER_DESC �Ķ��� bd
//    D3D11_BUFFER_DESC bd;
//    ZeroMemory(&bd, sizeof(bd));                   // �����������
//    bd.Usage = D3D11_USAGE_DEFAULT;       // ���û���Ķ�д��ʽ, һ����Ĭ�Ϸ�ʽ
//    bd.ByteWidth = sizeof(Vertex) * 3;        // ���û�������Ĵ�С,
//                                                   // ������������������Ҫ���� 3
//    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // �������������󶨵����㻺��
//    bd.CPUAccessFlags = 0;                         // CPU ���ʱ�ʶ��, 0 ��ʾû�� CPU ����
//    bd.MiscFlags = 0;                         // �������ʶ��, 0 ��ʾ��ʹ�ø���
//
//    // ����һ���������ڳ�ʼ������Դ
//    D3D11_SUBRESOURCE_DATA initData;
//    ZeroMemory(&initData, sizeof(initData));       // �����������
//    initData.pSysMem = vertices;                  // ������Ҫ��ʼ��������, ����������
//
//    // ����һ�� ID3D11Buffer ������Ϊ���㻺��
//    ID3D11Buffer* vertexBuffer;
//
//    // ���� CreateBuffer ��������
//    device->CreateBuffer(&bd, &initData, &vertexBuffer);
//    UINT stride = sizeof(Vertex);     // ��ȡ Vertex �Ĵ�С��Ϊ���
//    UINT offset = 0;                  // ����ƫ����Ϊ 0
//
//    // ���ö��㻺��
//    immediateContext->IASetVertexBuffers(
//        0,              // �󶨵���һ�������
//        1,              // ���㻺��ĸ���, ����Ϊһ��
//        &vertexBuffer,  // �����õĶ��㻺��
//        &stride,        // ���, ������ṹ�Ĵ�С
//        &offset);       // �����һ��Ԫ�ص�����Ԫ�ص�ƫ����
//
//// ָ��ͼԪ����, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ��ʾͼԪΪ������
//    immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//    return true;
//}
//
//void Cleanup()
//{
//    // �ͷ�ָ��
//    if (renderTargetView)
//    {
//        renderTargetView->Release();
//    }
//    if (immediateContext)
//    {
//        immediateContext->Release();
//    }
//    if (swapChain)
//    {
//        swapChain->Release();
//    }
//    if (device)
//    {
//        device->Release();
//    }
//    if (vertexShader)
//    {
//        vertexShader->Release();
//    }
//    if (pixelShader)
//    {
//        pixelShader->Release();
//    }
//}
//
//bool Display(float timeDelta)
//{
//    if (device)
//    {
//        // ����һ����������ɫ��Ϣ, 4 ��Ԫ�طֱ��ʾ��, ��, ���Լ� alpha
//        float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
//
//        // �����ȾĿ����ͼ
//        immediateContext->ClearRenderTargetView(renderTargetView,
//            ClearColor);
//
//        // �Ѵ����õĶ�����ɫ����������ɫ���󶨵� immediateContext
//        immediateContext->VSSetShader(vertexShader, NULL, 0);
//        immediateContext->PSSetShader(pixelShader, NULL, 0);
//
//        // ����������, ��һ��������ʾ���� 3 ����
//        // �ڶ���������ʾ�ӵ� 0 ���㿪ʼ����
//        immediateContext->Draw(3, 0);
//
//        swapChain->Present(0, 0);
//    }
//
//    return true;
//}
////************* ��ܺ�����д���� ***************
//
///*
//* �ص�����
//*/
//LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg,
//    WPARAM wParam, LPARAM lParam)
//{
//    switch (msg)
//    {
//    case WM_DESTROY:
//        ::PostQuitMessage(0);
//        break;
//    case WM_KEYDOWN:
//        if (VK_ESCAPE == wParam)
//        {
//            ::DestroyWindow(hwnd);
//        }
//        break;
//    }
//
//    return ::DefWindowProc(hwnd, msg, wParam, lParam);
//}
//
///*
//* ������ WinMain
//*/
//int WINAPI WinMain(HINSTANCE hInstance,
//    HINSTANCE hPreInstance,
//    PSTR pCmdLine,
//    int nShowCmd)
//{
//    // ��ʼ��
//    // ���������� 4 ��ָ��, ��������Ϊ�������� InitD3D ����
//    if (!d3d::InitD3D(hInstance,
//        800,
//        600,
//        &renderTargetView,
//        &immediateContext,
//        &swapChain,
//        &device))
//    {
//        ::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
//
//        return 0;
//    }
//    if (!Setup())
//    {
//        ::MessageBox(0, L"Setup() - FAILED", 0, 0);
//
//        return 0;
//    }
//
//    // ִ����Ϣѭ��, ������ Display ��ָ����Ϊ��������
//    d3d::EnterMsgLoop(Display);
//
//    Cleanup();
//
//    return 0;
//}