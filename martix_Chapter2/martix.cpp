//#if 0
//本例由于需要使用XNA函数库
//所以需要包含＂d3 dcompiler.h＂和＂xnamath.h＂
#include<iostream>
#include <d3dcompiler.h>
#include <xnamath.h>
#include<Windows.h>
using namespace std;
//重载＂<<＂操作符，让XMVECTOR的对象也可以使用＂cout<<＂进行输出，
//向量会以（X，X，X）形式输出到屏幕上
ostream& operator << (ostream& os, XMVECTOR u)
{
	//XMVectorGetX（），XMVectorGetY（），XMVectorGetZ（），XMVectorGetW（）
	   //这4个函数用来获取XMVECTOR的4个分量
	os << "(" << XMVectorGetX(u) << ","
		<< XMVectorGetY(u) << ","
		<< XMVectorGetZ(u) << ","
		<< XMVectorGetW(u) << ")"
		<< endl;
	return os;
}

//重载＂<<＂操作符，让XMFLOAT4X4的对象也可以使用＂cout<<＂进行输出，
//矩阵会以行-列的形式输出到屏幕上
//注意：这里使用XMFLOAT4X4对象作为参数而不是XMMATRIX作为参数
//　　这是由于因为系统（x64/x86）不同会存在对齐的问题，详细说明
//　　参考补充知识。使用XMMATRIX会报以下错误
//　　errorC2719：“m”：具有__declspec（align（′16′））的形参将不被对齐
//　　大家可以试一下，如果这里使用XMMATRIX作为参数会有什么结果
ostream& operator << (ostream& os, XMFLOAT4X4 m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//通过XMFLOAT4X4的重载括号操作符引用矩阵元素
			os << "\t" << m(i, j) << " ";
		}
		os << endl;
	}
	os << endl;
	return os;
}
int main()
{
	//声明3个XMMATRIX对象，
	   //分别用来表示平移矩阵（mTrans），旋转矩阵（mRota），
	   //以及缩放矩阵（mScal）
	XMMATRIX mTrans, mRota, mScal;

	//第一步：生成缩放矩阵
	//调用XMMatrixScaling（）函数用以生成缩放矩阵，该函数3个参数分别表示
	//在X，Y，Z轴上的缩放量。
	//在X，Y，Z轴缩小到1/5（即0.2），然后将生成的缩放矩阵赋值给mScal
	mScal = XMMatrixScaling(0.2f, 0.2f, 0.2f);

	//将生成的缩放矩阵打印到控制台上，这里只是方便我们查看生成的矩阵
	cout << "缩放矩阵为：" << endl;
	//由于重载的输出操作符<<是针对XMFLOAT4X4对象，所以这里要将XMMATRIX
	 //对象转换为XMFLOAT4X4对象
	 //首先声明一个XMFLOAT4X4对象
	XMFLOAT4X4 mScalFL;
	//利用XMStoreFloat4x4函数把mScal的内容存入XMFLOAT4X4对象mScalFL中
	XMStoreFloat4x4(&mScalFL, mScal);
	cout << mScalFL;
	//第二步：生成旋转矩阵
	//绕Y轴旋转45度，即1/4PI
	//调用XMMatrixRotationY（）函数用以生成旋转矩阵，该函数的参数为旋转的弧度
	//XM_PIDIV4为XNA库定义的数据常量表示1/4PI
	mRota = XMMatrixRotationY(XM_PIDIV4);

	//将生成的旋转矩阵打印到控制台上，方法同代码3.4，这里不再赘述
	cout << "旋转矩阵为：" << endl;
	XMFLOAT4X4 mRotaFL;
	XMStoreFloat4x4(&mRotaFL, mRota);
	cout << mRotaFL;
	//第三步：生成平移矩阵
	//在X轴平移1个单位，在Y轴平移2个单位，在Z轴平移-3个单位
	//调用函数XMMatrixTranslation生成平移矩阵，
	//该函数3个参数分别表示在X，Y，Z轴上的平移量
	mTrans = XMMatrixTranslation(1.0f, 2.0f, -3.0f);

	//将生成的旋转矩阵打印到控制台上，方法同代码3.4，这里不再赘述
	cout << "平移矩阵为：" << endl;
	XMFLOAT4X4 mTransFL;
	XMStoreFloat4x4(&mTransFL, mTrans);
	cout << mTransFL;
	//首先声明一个XMMATRIX对象用来存放最终的变换矩阵
	XMMATRIX mFinal;

	//利用XMMatrixMultiply来完成矩阵的相乘，
	//注意：由于矩阵相乘不具有交换性，所以做乘法时各个变换矩阵的顺序很重要
	//教材的例子的变换顺序是缩小（mScal）->旋转（mRota）->平移（mTrans）

	//所以这里首先将mScal和mRota相乘的中间结果放入mFinal中
	mFinal = XMMatrixMultiply(mScal, mRota);
	//再将中间结果与mTrans相乘，得到最终结果并覆盖先前的mFinal
	mFinal = XMMatrixMultiply(mFinal, mTrans);

	//将生成的变换矩阵打印到控制台上
	cout << "最终变换矩阵为:" << endl;
	XMFLOAT4X4 mFinalFL;
	XMStoreFloat4x4(&mFinalFL, mFinal);
	cout << mFinalFL;
	//按照例子声明一个XMVECTOR对象
   //例子中向量为3维向量，而XMVectorSet只能生成4维向量，
   //最后一个分量如果是1表示这是一个点，如果是0表示这是一个向量
   //这种向量称之为“齐次向量”，详细说明见补充知识。
	XMVECTOR vector = XMVectorSet(5.0f, 0.0f, 0.0f, 1.0f);

	//利用重载的操作符<<将声明的XMVECTOR对象打印到控制台上
	cout << "变换前的向量为：" << endl;
	cout << vector;

	//将上面生成的最终变换矩阵应用到XMVECTOR对象上
	//并将生成的新向量覆盖原来的向量
	vector = XMVector4Transform(vector, mFinal);

	//将最终的向量打印到控制台上
	cout << "变换后的向量为：" << endl;
	cout << vector;

	system("PAUSE");//让控制台不要闪退
	return 0;
}
//#endif
