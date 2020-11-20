//#if 0
//����������Ҫʹ��XNA������
//������Ҫ������d3 dcompiler.h���ͣ�xnamath.h��
#include<iostream>
#include <d3dcompiler.h>
#include <xnamath.h>
#include<Windows.h>
using namespace std;
//���أ�<<������������XMVECTOR�Ķ���Ҳ����ʹ�ã�cout<<�����������
//�������ԣ�X��X��X����ʽ�������Ļ��
ostream& operator << (ostream& os, XMVECTOR u)
{
	//XMVectorGetX������XMVectorGetY������XMVectorGetZ������XMVectorGetW����
	   //��4������������ȡXMVECTOR��4������
	os << "(" << XMVectorGetX(u) << ","
		<< XMVectorGetY(u) << ","
		<< XMVectorGetZ(u) << ","
		<< XMVectorGetW(u) << ")"
		<< endl;
	return os;
}

//���أ�<<������������XMFLOAT4X4�Ķ���Ҳ����ʹ�ã�cout<<�����������
//���������-�е���ʽ�������Ļ��
//ע�⣺����ʹ��XMFLOAT4X4������Ϊ����������XMMATRIX��Ϊ����
//��������������Ϊϵͳ��x64/x86����ͬ����ڶ�������⣬��ϸ˵��
//�����ο�����֪ʶ��ʹ��XMMATRIX�ᱨ���´���
//����errorC2719����m��������__declspec��align����16�䣩�����βν���������
//������ҿ�����һ�£��������ʹ��XMMATRIX��Ϊ��������ʲô���
ostream& operator << (ostream& os, XMFLOAT4X4 m)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//ͨ��XMFLOAT4X4���������Ų��������þ���Ԫ��
			os << "\t" << m(i, j) << " ";
		}
		os << endl;
	}
	os << endl;
	return os;
}
int main()
{
	//����3��XMMATRIX����
	   //�ֱ�������ʾƽ�ƾ���mTrans������ת����mRota����
	   //�Լ����ž���mScal��
	XMMATRIX mTrans, mRota, mScal;

	//��һ�����������ž���
	//����XMMatrixScaling�������������������ž��󣬸ú���3�������ֱ��ʾ
	//��X��Y��Z���ϵ���������
	//��X��Y��Z����С��1/5����0.2����Ȼ�����ɵ����ž���ֵ��mScal
	mScal = XMMatrixScaling(0.2f, 0.2f, 0.2f);

	//�����ɵ����ž����ӡ������̨�ϣ�����ֻ�Ƿ������ǲ鿴���ɵľ���
	cout << "���ž���Ϊ��" << endl;
	//�������ص����������<<�����XMFLOAT4X4������������Ҫ��XMMATRIX
	 //����ת��ΪXMFLOAT4X4����
	 //��������һ��XMFLOAT4X4����
	XMFLOAT4X4 mScalFL;
	//����XMStoreFloat4x4������mScal�����ݴ���XMFLOAT4X4����mScalFL��
	XMStoreFloat4x4(&mScalFL, mScal);
	cout << mScalFL;
	//�ڶ�����������ת����
	//��Y����ת45�ȣ���1/4PI
	//����XMMatrixRotationY������������������ת���󣬸ú����Ĳ���Ϊ��ת�Ļ���
	//XM_PIDIV4ΪXNA�ⶨ������ݳ�����ʾ1/4PI
	mRota = XMMatrixRotationY(XM_PIDIV4);

	//�����ɵ���ת�����ӡ������̨�ϣ�����ͬ����3.4�����ﲻ��׸��
	cout << "��ת����Ϊ��" << endl;
	XMFLOAT4X4 mRotaFL;
	XMStoreFloat4x4(&mRotaFL, mRota);
	cout << mRotaFL;
	//������������ƽ�ƾ���
	//��X��ƽ��1����λ����Y��ƽ��2����λ����Z��ƽ��-3����λ
	//���ú���XMMatrixTranslation����ƽ�ƾ���
	//�ú���3�������ֱ��ʾ��X��Y��Z���ϵ�ƽ����
	mTrans = XMMatrixTranslation(1.0f, 2.0f, -3.0f);

	//�����ɵ���ת�����ӡ������̨�ϣ�����ͬ����3.4�����ﲻ��׸��
	cout << "ƽ�ƾ���Ϊ��" << endl;
	XMFLOAT4X4 mTransFL;
	XMStoreFloat4x4(&mTransFL, mTrans);
	cout << mTransFL;
	//��������һ��XMMATRIX��������������յı任����
	XMMATRIX mFinal;

	//����XMMatrixMultiply����ɾ������ˣ�
	//ע�⣺���ھ�����˲����н����ԣ��������˷�ʱ�����任�����˳�����Ҫ
	//�̲ĵ����ӵı任˳������С��mScal��->��ת��mRota��->ƽ�ƣ�mTrans��

	//�����������Ƚ�mScal��mRota��˵��м�������mFinal��
	mFinal = XMMatrixMultiply(mScal, mRota);
	//�ٽ��м�����mTrans��ˣ��õ����ս����������ǰ��mFinal
	mFinal = XMMatrixMultiply(mFinal, mTrans);

	//�����ɵı任�����ӡ������̨��
	cout << "���ձ任����Ϊ:" << endl;
	XMFLOAT4X4 mFinalFL;
	XMStoreFloat4x4(&mFinalFL, mFinal);
	cout << mFinalFL;
	//������������һ��XMVECTOR����
   //����������Ϊ3ά��������XMVectorSetֻ������4ά������
   //���һ�����������1��ʾ����һ���㣬�����0��ʾ����һ������
   //����������֮Ϊ���������������ϸ˵��������֪ʶ��
	XMVECTOR vector = XMVectorSet(5.0f, 0.0f, 0.0f, 1.0f);

	//�������صĲ�����<<��������XMVECTOR�����ӡ������̨��
	cout << "�任ǰ������Ϊ��" << endl;
	cout << vector;

	//���������ɵ����ձ任����Ӧ�õ�XMVECTOR������
	//�������ɵ�����������ԭ��������
	vector = XMVector4Transform(vector, mFinal);

	//�����յ�������ӡ������̨��
	cout << "�任�������Ϊ��" << endl;
	cout << vector;

	system("PAUSE");//�ÿ���̨��Ҫ����
	return 0;
}
//#endif
