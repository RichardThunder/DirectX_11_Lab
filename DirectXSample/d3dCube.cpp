#include "d3dUtility.h"


//声明全局的指针
ID3D11Device* device = NULL;//D3D11设备接口
IDXGISwapChain* swapChain = NULL;//交换链接口
ID3D11DeviceContext* immediateContext = NULL;
ID3D11RenderTargetView* renderTargetView = NULL;//渲染目标视图  

//着色器
ID3D11VertexShader* m_VertexShader;
ID3D11PixelShader* m_PixelShader;

//Effect相关全局指针
ID3D11InputLayout* vertexLayout;
ID3DX11Effect* effect;
ID3DX11EffectTechnique* technique;

//声明三个坐标系矩阵
XMMATRIX world;         //用于世界变换的矩阵
XMMATRIX view;          //用于观察变换的矩阵
XMMATRIX projection;    //用于投影变换的矩阵

//定义一个顶点结构，这个顶点包含坐标和颜色信息
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

//**************以下为框架函数******************
bool Setup()
{
	//这里要创建一个Cube主要包含3个主要步骤
	//第一步从.fx文件创建ID3DEffect对象
	//第二步创建顶点缓存以及顶点索引缓存
	//第三步设置变换坐标系
	//*************第一步从.fx文件创建ID3DEffect对象****************************
	HRESULT hr = S_OK;              //声明HRESULT的对象用于记录函数调用是否成功
	ID3DBlob* pTechBlob = NULL;     //声明ID3DBlob的对象用于存放从文件读取的信息
	//从我们之前建立的.fx文件读取着色器相关信息，该函数的注释见实验4，
	//这里仅仅是修改了读取文件的文件名
	hr = D3DX11CompileFromFile(L"SimpleShader.fx", NULL, NULL, NULL, "fx_5_0",
		D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &pTechBlob, NULL, NULL);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"fx文件载入失败", L"Error", MB_OK); //如果读取失败，弹出错误信息
		return hr;
	}
	//调用D3DX11CreateEffectFromMemory创建ID3DEffect对象
	hr = D3DX11CreateEffectFromMemory(
		pTechBlob->GetBufferPointer(),    //从.fx文件读入信息所在内存的起始位置
		pTechBlob->GetBufferSize(),       //从.fx文件读入信息所在内存的大小
		0,                                //一般设置为0，表示无Effect标识
		device,                           //设备指针
		&effect);                         //返回创建好的ID3DEffect对象

	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建Effect失败", L"Error", MB_OK);  //创建失败，弹出错误信息
		return hr;
	}
	//调用GetTechniqueByIndex获取ID3DX11EffectTechnique的对象
	//参数为一个从0开始的索引
	technique = effect->GetTechniqueByIndex(0);

	//D3DX11_PASS_DESC结构用于描述一个Effect Pass
	D3DX11_PASS_DESC PassDesc;
	//利用GetPassByIndex获取Effect Pass
	//再利用GetDesc获取Effect Pass的描述，并存如PassDesc对象中
	technique->GetPassByIndex(0)->GetDesc(&PassDesc);

	//创建并设置输入布局
	//这里我们定义一个D3D11_INPUT_ELEMENT_DESC数组，
	//由于我们定义的顶点结构包括位置坐标和颜色，所以这个数组里有两个元素
	//比实验4中的输入布局多了一个元素
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	//layout元素个数
	UINT numElements = ARRAYSIZE(layout);
	//调用CreateInputLayout创建输入布局
	hr = device->CreateInputLayout(
		layout,                         //上面定义的D3D11_INPUT_ELEMENT_DESC数组
		numElements,                    //D3D11_INPUT_ELEMENT_DESC数组的元素个数
		PassDesc.pIAInputSignature,     //和实验4不同，这个参数传入的是Effect Pass描述的输入标识
		PassDesc.IAInputSignatureSize,  //和实验4不同，这个参数传入的是Effect Pass描述的输入标识的大小
		&vertexLayout);                //返回生成的输入布局对象
//设置生成的输入布局到设备上下文中
	immediateContext->IASetInputLayout(vertexLayout);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建Input Layout失败", L"Error", MB_OK);
		return hr;
	}
	//*************第一步从.fx文件创建ID3DEffect对象****************************
	//*************第二步创建顶点缓存以及顶点索引缓存***************************
	//和实验4一样，创建顶点数组，由于每个顶点有包含了坐标和颜色
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
	//创建顶点缓存，方法同实验4一样
	//首先声明一个D3D11_BUFFER_DESC的对象bd
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * 8;      //注意：由于这里定义了8个顶点所以要乘以8
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  //注意：这里表示创建的是顶点缓存
	bd.CPUAccessFlags = 0;

	//声明一个D3D11_SUBRESOURCE_DATA数据用于初始化子资源
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;         //设置需要初始化的数据，这里的数据就是顶点数组

	//声明一个ID3D11Buffer对象作为顶点缓存
	ID3D11Buffer* vertexBuffer;
	//调用CreateBuffer创建顶点缓存
	hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建VertexBuffer失败", L"Error", MB_OK);
		return hr;
	}
	//设置索引数组
	//注意：数组里的每一个数字表示顶点数组的对应下标的顶点。
	//      由于立方体由12个三角形组成，所以共需要36个顶点
	//      这里每三个数字构成一个三角形
	WORD indices[] =
	{
		3,1,0,   2,1,3,   0,5,4,   1,5,0,   3,4,7,   0,4,3,
		1,6,5,   2,6,1,   2,7,6,   3,7,2,   6,4,5,   7,4,6,
	};
	UINT indexCount = ARRAYSIZE(indices);

	//创建索引缓存
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;        // 由于有36个顶点所以这里要乘以36
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;    // 注意：这里表示创建的是索引缓存
	bd.CPUAccessFlags = 0;

	InitData.pSysMem = indices;                //设置需要初始化的数据，这里的数据就是索引数组
	ID3D11Buffer* indexBuffer;                 //声明一个ID3D11Buffer对象作为索引缓存
	//调用CreateBuffer创建索引缓存
	hr = device->CreateBuffer(&bd, &InitData, &indexBuffer);
	if (FAILED(hr))
	{
		::MessageBox(NULL, L"创建IndexBuffer失败", L"Error", MB_OK);
		return hr;
	}
	UINT stride = sizeof(Vertex);                 //获取Vertex的大小作为跨度
	UINT offset = 0;                              //设置偏移量为0
	//设置顶点缓存，参数的解释见实验4
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//设置索引缓存
	immediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	//指定图元类型，D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST表示图元为三角形
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//*************第二步创建顶点缓存以及顶点索引缓存****************************

	//*************第三步设置变换坐标系***********************
	//初始化世界矩阵
	world = XMMatrixIdentity();

	//初始化观察矩阵
	//XMVECTOR Eye = XMVectorSet( 3.0f, 5.0f, -5.0f, 0.0f );//相机位置
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -15.0f, 0.0f);//相机位置
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);  //目标位置
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);  //up
	view = XMMatrixLookAtLH(Eye, At, Up);   //设置观察坐标系


	//设置投影矩阵
	projection = XMMatrixPerspectiveFovLH(
		XM_PIDIV2,            //视野夹角
		800.0f / 600.0f,      //宽高比
		0.01f,                //近裁剪面距离
		100.0f);             //远裁剪面距离
//*************第三步设置变换坐标系***********************
	return true;
}

void Cleanup()
{
	//释放全局指针
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
		//声明一个数组存放颜色信息，4个元素分别表示红，绿，蓝以及alpha
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		//float ClearColor[4] = { 0.0f, 0.125f, 0.0f, 1.0f };
		immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);

		//将几个坐标系变换矩阵设置到Effect框架中
		//注意：这里的"World"，"View"，"Projection"是在.fx文件中定义的
		effect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&world);  //设置世界坐标系
		effect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&view);    //设置观察坐标系
		effect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&projection); //设置投影坐标系

		//定义一个D3DX11_TECHNIQUE_DESC对象来描述technique
		D3DX11_TECHNIQUE_DESC techDesc;
		technique->GetDesc(&techDesc);    //获取technique的描述
		//获取通道（PASS）把它设置到设备上下文中。
		//这里由于只有一个通道所以其索引为0
		technique->GetPassByIndex(0)->Apply(0, immediateContext);

		//绘制立方体，注意是这里调用的是DrawIndexed!
		//第一个参数3表示绘制3个点，
		//第二个参数0表示从第0个索引开始绘制
		//第三个参数表示从顶点缓存读出一个顶点时，给索引增加一个值，一般为0
		immediateContext->DrawIndexed(36, 0, 0);

		swapChain->Present(0, 0);

		// 每隔一段时间更新一次场景，以实现立方体的旋转.
		static float angle = 0.0f;   //声明一个静态变量用于记录角度
		angle += timeDelta;          //将当前角度加上一个时间差
		if (angle >= 6.28f)           //如果当前角度大于2PI，则归零
			angle = 0.0f;
		world = XMMatrixIdentity();

		//world = XMMatrixRotationY( angle );   //根据当前角度设置世界坐标系
		//world = XMMatrixRotationX( angle )*XMMatrixRotationY( angle )*XMMatrixRotationZ( angle );   //绕3个轴旋转
		world = XMMatrixRotationZ(angle)*XMMatrixTranslation(6.0f, 0.0f, 0.0f) * XMMatrixRotationZ(angle) ;
		//world = XMMatrixRotationY(angle) * XMMatrixTranslation(/*6*/0.0f, 0.0f, 0.0f) ;
		/*实现逆时针旋转
		angle -= timeDelta;          //将当前角度加上一个时间差
		if(angle <= -6.28f)           //如果当前角度大于2PI，则归零
			angle = 0.0f;
		world = XMMatrixRotationY( angle );   //根据当前角度设置世界坐标系
		*/
		immediateContext->DrawIndexed(36, 0, 0);

		swapChain->Present(0, 0);

		// 每隔一段时间更新一次场景，以实现立方体的旋转.
		static float angle1 = 3.14f;   //声明一个静态变量用于记录角度
		angle1 += timeDelta;          //将当前角度加上一个时间差
		if (angle1 >= 6.28f)           //如果当前角度大于2PI，则归零
			angle1 = 0.0f;
		//world = XMMatrixRotationY( angle );   //根据当前角度设置世界坐标系
		//world = XMMatrixRotationX( angle )*XMMatrixRotationY( angle )*XMMatrixRotationZ( angle );   //绕3个轴旋转
		world = XMMatrixRotationZ(angle1) * XMMatrixTranslation(-6.0f, 0.0f, 0.0f) * XMMatrixRotationZ(angle1);
	}
	return true;
}
//**************框架函数******************


//
// 回调函数
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
// 主函数WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{

	//初始化
	//**注意**:最上面声明的IDirect3DDevice9指针，在这里作为参数传给InitD3D函数
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