#include <Windows.h>
#include <windowsx.h>
#include "d3dFramework.h"
#define  SAFE_DEl(x)  if(x){x->Release();}
bool Display(float time, IDirect3DDevice9* Device_);
LRESULT CALLBACK WndProc(HWND hwnd, UINT ui, WPARAM wparam, LPARAM lparam);

IDirect3DDevice9* Device = 0;
const int _width = 800;
const int _height = 600;
IDirect3DVertexBuffer9* Triangle = 0;
IDirect3DIndexBuffer9*  Ib = 0;
struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z)
	{
		_x = x;
		_y = y;
		_z = z;
	}

	float _x, _y, _z;
	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ;
void Construct();
void Destruct();
void Construct_word();

int WINAPI  WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{

	//IDirect3DDevice9* device_;
	//设备创建和初始化
	if (!d3d9Initial(hInstance, (WNDPROC)WndProc, _width, _height, false, D3DDEVTYPE_HAL,&Device))
	{
		MessageBox(0, "d3dinitial failed", 0, 0);
		return 0;
	}

	Construct();

	d3d9MessageLoop(Display, Device);

	Destruct();

	SAFE_DEl(Device)
	
	return 0;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT ui, WPARAM wparam, LPARAM lparam )
{


	switch(ui)
	{
	 case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}
	//自己不处理的消息一定要交给默认的处理过程！！
	//要不然窗口显示不出来
	return DefWindowProc(hwnd, ui, wparam, lparam);
}

bool Display( float time, IDirect3DDevice9* Device_ )
{
	if (Device_)
	{
		D3DXMATRIX ry;
		static float y = 0.0f;
		D3DXMatrixRotationY(&ry, y);
        y += 0.0002;
		if (y>6.28)
		{
			y = 0.0f;
		}

		Device_->SetTransform(D3DTS_WORLD, &ry);
		Device_->Clear(0,0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(230,230,230), 1.0f, 0);
		Device_->BeginScene();

		//Device_->SetStreamSource(0, Triangle, 0, sizeof(Vertex));
		//Device_->SetFVF(Vertex::FVF);
		//Device_->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 8);

		Device_->SetStreamSource(0, Triangle, 0, sizeof(Vertex));
		Device_->SetIndices(Ib);
		Device_->SetFVF(Vertex::FVF);
		Device_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6,0,8);
		Device_->EndScene();
		Device_->Present(0,0,0,0);
	}
    
	return true;
}

void Construct()
{
	//顶点缓存
	Device->CreateVertexBuffer(
		 6*sizeof(Vertex),
		 D3DUSAGE_WRITEONLY,
		 Vertex::FVF,
		 D3DPOOL_MANAGED,
		 &Triangle,
		 0);

	//索引缓存
	Device->CreateIndexBuffer(
			24*sizeof(WORD),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&Ib,
			0);
	//填充顶点缓存
	Vertex* v;
	Triangle->Lock(0,0,(void**)&v, 0);
	v[0] = Vertex(0.0f, 1.4f, 0.0f);
	v[1] = Vertex(-1.0f, 0.0f, -1.0f);
	v[2] = Vertex(-1.0f, 0.0f, 1.0f);
	v[3] = Vertex(1.0f, 0.0f, 1.0f);
	v[4] = Vertex(1.0f, 0.0f, -1.0f);
	v[5] = Vertex(0.0f, -1.4f, 0.0f);
// 	v[0] = Vertex(0.0f, 1.4f, 0.0f);
// 	v[1] = Vertex(-1.0f, 0.0f, -1.0f);
// 	v[2] = Vertex(-1.0f, 0.0f, 1.0f);
// 	v[3] = Vertex(0.0f, 1.4f, 0.0f);
// 	v[4] = Vertex(-1.0f, 0.0f, 1.0f);
// 	v[5] = Vertex(1.0f, 0.0f, 1.0f);
// 	v[6] = Vertex(0.0f, 1.4f, 0.0f);
// 	v[7] = Vertex(1.0f, 0.0f, 1.0f);
// 	v[8] = Vertex(1.0f, 0.0f, -1.0f);
// 	v[9] = Vertex(0.0f, 1.4f, 0.0f);
// 	v[10] = Vertex(1.0f, 0.0f, -1.0f);
// 	v[11] = Vertex(-1.0f, 0.0f, -1.0f);
// 	v[12] = Vertex(0.0f, -1.4f, 0.0f);
// 	v[13] = Vertex(-1.0f, 0.0f, -1.0f);
// 	v[14] = Vertex(1.0f, 0.0f, -1.0f);
// 	v[15] = Vertex(0.0f, -1.4f, 0.0f);
// 	v[16] = Vertex(1.0f, 0.0f, -1.0f);
// 	v[17] = Vertex(1.0f, 0.0f, 1.0f);
// 	v[18] = Vertex(0.0f, -1.4f, 0.0f);
// 	v[19] = Vertex(1.0f, 0.0f, 1.0f);
// 	v[20] = Vertex(-1.0f, 0.0f, 1.0f);
// 	v[21] = Vertex(0.0f, -1.4f, 0.0f);
// 	v[22] = Vertex(-1.0f, 0.0f, 1.0f);
// 	v[23] = Vertex(-1.0f, 0.0f, -1.0f);
	Triangle->Unlock();

	WORD* indices = 0;
	Ib->Lock(0,0,(void**)&indices,0);

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;
	indices[6] = 0; indices[7] = 3; indices[8] = 4;
	indices[9] = 0; indices[10] = 4; indices[11] = 1;
	indices[12] = 5; indices[13] = 1; indices[14] = 4;
	indices[15] = 5; indices[16] = 4; indices[17] = 3;
	indices[18] = 5; indices[19] = 3; indices[20] = 2;
	indices[21] = 5; indices[22] = 2; indices[23] = 1;
	Ib->Unlock();
	//定义摄像机
	D3DXVECTOR3  pos(2.0f, 1.0f, -3.0f);
	D3DXVECTOR3  target(0.0, 0.0, 0.0);
	D3DXVECTOR3  up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX   V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &V);

	//投影变换
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
								&proj,
								D3DX_PI*0.5,
								(float)_width/(float)_height,
								1.0f,
								1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	//线框模型
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

}

void Destruct()
{
	if (Triangle)
	{
		Triangle->Release();
	}

	if (Ib)
	{
		Ib->Release();
	}

}

void Construct_word()
{
	HDC hdc = ::CreateCompatibleDC(0); //创建设备环境

	HFONT hfont = CreateFont(10,
							10,
							0,
							0,
							1000,
							0,
							0,
							0,
							DEFAULT_CHARSET,
							0,0,0,0,
							"Times New Roman");

}
