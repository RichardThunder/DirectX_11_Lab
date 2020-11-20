#include "d3dUtility.h"


//����ȫ�ֵ�ָ��
ID3D11Device* device = NULL;//D3D11�豸�ӿ�
IDXGISwapChain* swapChain = NULL;//�������ӿ�
ID3D11DeviceContext* immediateContext = NULL;
ID3D11RenderTargetView* renderTargetView = NULL;//��ȾĿ����ͼ  

//��ɫ��
ID3D11VertexShader* m_VertexShader;
ID3D11PixelShader* m_PixelShader;

//Effect���ȫ��ָ��
ID3D11InputLayout* vertexLayout;
ID3DX11Effect* effect;
ID3DX11EffectTechnique* technique;

//������������ϵ����
XMMATRIX world;         //��������任�ľ���
XMMATRIX view;          //���ڹ۲�任�ľ���
XMMATRIX projection;    //����ͶӰ�任�ľ���

//����һ������ṹ�������������������ɫ��Ϣ
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

//**************����Ϊ��ܺ���******************
bool Setup()
{
	//����Ҫ����һ��Cube��Ҫ����3����Ҫ����
	//��һ����.fx�ļ�����ID3DEffect����
	//�ڶ����������㻺���Լ�������������
	//���������ñ任����ϵ
	//*************��һ����.fx�ļ�����ID3DEffect����****************************
	HRESULT hr = S_OK;              //����HRESULT�Ķ������ڼ�¼���������Ƿ�ɹ�
	ID3DBlob* pTechBlob = NULL;     //����ID3DBlob�Ķ������ڴ�Ŵ��ļ���ȡ����Ϣ
	//������֮ǰ������.fx�ļ���ȡ��ɫ�������Ϣ���ú�����ע�ͼ�ʵ��4��
	//����������޸��˶�ȡ�ļ����ļ���
	hr = D3DX11CompileFromFile(L"SimpleShader.fx", NULL, NULL, NULL, "fx_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &pTechBlob, NULL, NULL);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"fx�ļ�����ʧ��", L"Error", MB_OK); //�����ȡʧ�ܣ�����������Ϣ
		return hr;
	}
	//����D3DX11CreateEffectFromMemory����ID3DEffect����
	hr = D3DX11CreateEffectFromMemory(
		pTechBlob->GetBufferPointer(),    //��.fx�ļ�������Ϣ�����ڴ����ʼλ��
		pTechBlob->GetBufferSize(),       //��.fx�ļ�������Ϣ�����ڴ�Ĵ�С
		0,                                //һ������Ϊ0����ʾ��Effect��ʶ
		device,                           //�豸ָ��
		&effect);                         //���ش����õ�ID3DEffect����

	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����Effectʧ��", L"Error", MB_OK);  //����ʧ�ܣ�����������Ϣ
		return hr;
	}
	//����GetTechniqueByIndex��ȡID3DX11EffectTechnique�Ķ���
	//����Ϊһ����0��ʼ������
	technique = effect->GetTechniqueByIndex(0);

	//D3DX11_PASS_DESC�ṹ��������һ��Effect Pass
	D3DX11_PASS_DESC PassDesc;
	//����GetPassByIndex��ȡEffect Pass
	//������GetDesc��ȡEffect Pass��������������PassDesc������
	technique->GetPassByIndex(0)->GetDesc(&PassDesc);

	//�������������벼��
	//�������Ƕ���һ��D3D11_INPUT_ELEMENT_DESC���飬
	//�������Ƕ���Ķ���ṹ����λ���������ɫ���������������������Ԫ��
	//��ʵ��4�е����벼�ֶ���һ��Ԫ��
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	//layoutԪ�ظ���
	UINT numElements = ARRAYSIZE(layout);
	//����CreateInputLayout�������벼��
	hr = device->CreateInputLayout(
		layout,                         //���涨���D3D11_INPUT_ELEMENT_DESC����
		numElements,                    //D3D11_INPUT_ELEMENT_DESC�����Ԫ�ظ���
		PassDesc.pIAInputSignature,     //��ʵ��4��ͬ����������������Effect Pass�����������ʶ
		PassDesc.IAInputSignatureSize,  //��ʵ��4��ͬ����������������Effect Pass�����������ʶ�Ĵ�С
		&vertexLayout);                //�������ɵ����벼�ֶ���
//�������ɵ����벼�ֵ��豸��������
	immediateContext->IASetInputLayout(vertexLayout);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����Input Layoutʧ��", L"Error", MB_OK);
		return hr;
	}
	//*************��һ����.fx�ļ�����ID3DEffect����****************************
	//*************�ڶ����������㻺���Լ�������������***************************
	//��ʵ��4һ���������������飬����ÿ�������а������������ɫ
	Vertex vertices[] =
	{
		{ XMFLOAT3(-3.0f, 3.0f, -3.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 3.0f) },
		{ XMFLOAT3(3.0f, 3.0f, -3.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 3.0f) },
		{ XMFLOAT3(3.0f, 3.0f, 3.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 3.0f) },
		{ XMFLOAT3(-3.0f, 3.0f, 3.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 3.0f) },
		{ XMFLOAT3(-3.0f, -3.0f, -3.0f), XMFLOAT4(3.0f, 0.0f, 3.0f, 3.0f) },
		{ XMFLOAT3(3.0f, -3.0f, -3.0f), XMFLOAT4(3.0f, 3.0f, 0.0f, 3.0f) },
		{ XMFLOAT3(3.0f, -3.0f, 3.0f), XMFLOAT4(3.0f, 3.0f, 3.0f, 3.0f) },
		{ XMFLOAT3(-3.0f, -3.0f, 3.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 3.0f) },
	};
	UINT vertexCount = ARRAYSIZE(vertices);
	//�������㻺�棬����ͬʵ��4һ��
	//��������һ��D3D11_BUFFER_DESC�Ķ���bd
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * 8;      //ע�⣺�������ﶨ����8����������Ҫ����8
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  //ע�⣺�����ʾ�������Ƕ��㻺��
	bd.CPUAccessFlags = 0;

	//����һ��D3D11_SUBRESOURCE_DATA�������ڳ�ʼ������Դ
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;         //������Ҫ��ʼ�������ݣ���������ݾ��Ƕ�������

	//����һ��ID3D11Buffer������Ϊ���㻺��
	ID3D11Buffer* vertexBuffer;
	//����CreateBuffer�������㻺��
	hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����VertexBufferʧ��", L"Error", MB_OK);
		return hr;
	}
	//������������
	//ע�⣺�������ÿһ�����ֱ�ʾ��������Ķ�Ӧ�±�Ķ��㡣
	//      ������������12����������ɣ����Թ���Ҫ36������
	//      ����ÿ�������ֹ���һ��������
	WORD indices[] =
	{
		3,1,0,   2,1,3,   0,5,4,   1,5,0,   3,4,7,   0,4,3,
		1,6,5,   2,6,1,   2,7,6,   3,7,2,   6,4,5,   7,4,6,
	};
	UINT indexCount = ARRAYSIZE(indices);

	//������������
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;        // ������36��������������Ҫ����36
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;    // ע�⣺�����ʾ����������������
	bd.CPUAccessFlags = 0;

	InitData.pSysMem = indices;                //������Ҫ��ʼ�������ݣ���������ݾ�����������
	ID3D11Buffer* indexBuffer;                 //����һ��ID3D11Buffer������Ϊ��������
	//����CreateBuffer������������
	hr = device->CreateBuffer(&bd, &InitData, &indexBuffer);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"����IndexBufferʧ��", L"Error", MB_OK);
		return hr;
	}
	UINT stride = sizeof(Vertex);                 //��ȡVertex�Ĵ�С��Ϊ���
	UINT offset = 0;                              //����ƫ����Ϊ0
	//���ö��㻺�棬�����Ľ��ͼ�ʵ��4
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//������������
	immediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//ָ��ͼԪ���ͣ�D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST��ʾͼԪΪ������
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//*************�ڶ����������㻺���Լ�������������****************************

	//*************���������ñ任����ϵ***********************
	//��ʼ���������
	world = XMMatrixIdentity();

	//��ʼ���۲����
	//XMVECTOR Eye = XMVectorSet( 3.0f, 5.0f, -5.0f, 0.0f );//���λ��
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -15.0f, 0.0f);//���λ��
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);  //Ŀ��λ��
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);  //up
	view = XMMatrixLookAtLH(Eye, At, Up);   //���ù۲�����ϵ


	//����ͶӰ����
	projection = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,            //��Ұ�н�
		800.0f / 600.0f,      //��߱�
		0.01f,                //���ü������
		100.0f);             //Զ�ü������
//*************���������ñ任����ϵ***********************
	return true;
}

void Cleanup()
{
	//�ͷ�ȫ��ָ��
	if (renderTargetView) renderTargetView->Release();
	if (immediateContext) immediateContext->Release();
	if (swapChain) swapChain->Release();
	if (device) device->Release();
	if (m_VertexShader) m_VertexShader->Release();
	if (m_PixelShader) m_PixelShader->Release();

	if (vertexLayout) vertexLayout->Release();
	if (effect) effect->Release();
}

bool Display(float timeDelta)
{
	if (device)
	{
		//����һ����������ɫ��Ϣ��4��Ԫ�طֱ��ʾ�죬�̣����Լ�alpha
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		//float ClearColor[4] = { 0.0f, 0.125f, 0.0f, 1.0f };
		immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);

		//����������ϵ�任�������õ�Effect�����
		//ע�⣺�����"World"��"View"��"Projection"����.fx�ļ��ж����
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //������������ϵ
		effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);    //���ù۲�����ϵ
		effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection); //����ͶӰ����ϵ

		//����һ��D3DX11_TECHNIQUE_DESC����������technique
		D3DX11_TECHNIQUE_DESC techDesc;
		technique->GetDesc(&techDesc);    //��ȡtechnique������
		//��ȡͨ����PASS���������õ��豸�������С�
		//��������ֻ��һ��ͨ������������Ϊ0
		technique->GetPassByIndex(0)->Apply(0, immediateContext);

		//���������壬ע����������õ���DrawIndexed!
		//��һ������3��ʾ����3���㣬
		//�ڶ�������0��ʾ�ӵ�0��������ʼ����
		//������������ʾ�Ӷ��㻺�����һ������ʱ������������һ��ֵ��һ��Ϊ0
		immediateContext->DrawIndexed(36, 0, 0);

		swapChain->Present(0, 0);

		// ÿ��һ��ʱ�����һ�γ�������ʵ�����������ת.
		static float angle = 0.0f;   //����һ����̬�������ڼ�¼�Ƕ�
		angle += timeDelta;          //����ǰ�Ƕȼ���һ��ʱ���
		if (angle >= 6.28f)           //�����ǰ�Ƕȴ���2PI�������
			angle = 0.0f;
		world = XMMatrixIdentity();

		//world = XMMatrixRotationY( angle );   //���ݵ�ǰ�Ƕ�������������ϵ
		//world = XMMatrixRotationX( angle )*XMMatrixRotationY( angle )*XMMatrixRotationZ( angle );   //��3������ת
		world = XMMatrixRotationZ(angle)*XMMatrixTranslation(6.0f, 0.0f, 0.0f) * XMMatrixRotationZ(angle) ;
		//world = XMMatrixRotationY(angle) * XMMatrixTranslation(/*6*/0.0f, 0.0f, 0.0f) ;
		/*ʵ����ʱ����ת
		angle -= timeDelta;          //����ǰ�Ƕȼ���һ��ʱ���
		if(angle <= -6.28f)           //�����ǰ�Ƕȴ���2PI�������
			angle = 0.0f;
		world = XMMatrixRotationY( angle );   //���ݵ�ǰ�Ƕ�������������ϵ
		*/
		immediateContext->DrawIndexed(36, 0, 0);

		swapChain->Present(0, 0);

		// ÿ��һ��ʱ�����һ�γ�������ʵ�����������ת.
		static float angle1 = 3.14f;   //����һ����̬�������ڼ�¼�Ƕ�
		angle1 += timeDelta;          //����ǰ�Ƕȼ���һ��ʱ���
		if (angle1 >= 6.28f)           //�����ǰ�Ƕȴ���2PI�������
			angle1 = 0.0f;
		//world = XMMatrixRotationY( angle );   //���ݵ�ǰ�Ƕ�������������ϵ
		//world = XMMatrixRotationX( angle )*XMMatrixRotationY( angle )*XMMatrixRotationZ( angle );   //��3������ת
		world = XMMatrixRotationZ(angle1) * XMMatrixTranslation(-6.0f, 0.0f, 0.0f) * XMMatrixRotationZ(angle1);
	}
	return true;
}
//**************��ܺ���******************


//
// �ص�����
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

//
// ������WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{

	//��ʼ��
	//**ע��**:������������IDirect3DDevice9ָ�룬��������Ϊ��������InitD3D����
	if (!d3d::InitD3D(hinstance,
		800,
		600,
		&renderTargetView,
		&immediateContext,
		&swapChain,
		&device))// [out]The created device.
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	return 0;
}