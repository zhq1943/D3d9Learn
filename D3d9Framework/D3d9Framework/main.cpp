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

struct VertexColor
{
	VertexColor(){}
	VertexColor(float x, float y, float z, D3DCOLOR color)
	{
		x_ = x;
		y_ = y;
		z_ = z;
		color_ = color;
	}

	float x_;
	float y_;
	float z_;
	D3DCOLOR color_;
	static const DWORD FVFC;

};

struct VertexNor
{
	VertexNor(){}
	VertexNor(float x, float y, float z, float n_x, float n_y, float n_z)
	{
		x_ = x;
		y_ = y;
		z_ = z;
		nx = n_x;
		ny = n_y;
		nz = n_z;
	}

	float x_ , y_, z_, nx, ny, nz;
	static const DWORD FVFN;
};

IDirect3DIndexBuffer9* ib_nor = 0;
IDirect3DVertexBuffer9* vb_nor = 0;
RECT rec_t;

D3DXMATRIX WorldMatrix;
IDirect3DVertexBuffer9* VB_c = 0;
IDirect3DIndexBuffer9* IB_c = 0;

//3d文字
ID3DXMesh* Text = 0;
ID3DXFont* Font = NULL;

const DWORD Vertex::FVF = D3DFVF_XYZ;
const DWORD VertexColor::FVFC = D3DFVF_XYZ|D3DFVF_DIFFUSE;
const DWORD VertexNor::FVFN = D3DFVF_XYZ|D3DFVF_NORMAL;
void Construct();
void Destruct();
void Construct_word();
void Destruct_word();
void Construct_rect();
void Destruct_rect();
void Construct_LightModel();
void Destruct_LightModel();
D3DCOLORVALUE HelpFunc_Times(D3DCOLORVALUE& des, float times);
D3DLIGHT9 Initd3d9light(D3DXVECTOR3& dir, D3DXCOLOR &color);

int WINAPI  WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{

	//IDirect3DDevice9* device_;
	//设备创建和初始化
	if (!d3d9Initial(hInstance, (WNDPROC)WndProc, _width, _height, false, D3DDEVTYPE_HAL,&Device))
	{
		MessageBox(0, "d3dinitial failed", 0, 0);
		return 0;
	}

	//Construct_word();
	//Construct();
	//Construct_rect();
	Construct_LightModel();


	d3d9MessageLoop(Display, Device);

	//Destruct();
	//Destruct_word();
	//Destruct_rect();
	Destruct_LightModel();

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
        y += 0.00002f;
		if (y>6.28)
		{
			y = 0.0f;
		}

		Device_->SetTransform(D3DTS_WORLD, &ry);
		Device_->Clear(0,0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
		Device_->BeginScene();

		{   //顶点绘制
			Device_->SetStreamSource(0, vb_nor, 0, sizeof(VertexNor));
			Device_->SetFVF(VertexNor::FVFN);
			Device_->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);
		}

		{   //索引绘制
			// 		Device_->SetStreamSource(0, Triangle, 0, sizeof(Vertex));
			// 		Device_->SetIndices(Ib);
			// 		Device_->SetFVF(Vertex::FVF);
			// 		Device_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6,0,8);
		}

		{
			//绘制文字
			//Text->DrawSubset(0);
			//Font->DrawTextA(NULL, "2D", -1,&rec_t, DT_TOP|DT_LEFT, D3DXCOLOR(1.0, 0.8f,0.0f,1.0f));
		}

		{
			//draw a rect with color
			/*Device_->SetStreamSource(0, VB_c, 0, sizeof(VertexColor));
			Device_->SetIndices(IB_c);
			Device_->SetFVF(VertexColor::FVFC);

			D3DXMatrixTranslation(&WorldMatrix, -1.25f, 0.0f, 0.0f);
			Device_->SetTransform(D3DTS_WORLD, &WorldMatrix);
			Device_->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
			Device_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,4,0,2);

			D3DXMatrixTranslation(&WorldMatrix, 1.25f, 0.0f, 0.0f);
			Device_->SetTransform(D3DTS_WORLD, &WorldMatrix);
			Device_->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
			Device_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,4,0,2);*/
		}

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

	rec_t.left = 0L;
	rec_t.top = 0;
	rec_t.bottom = 100;
	rec_t.right = 200;

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

	HFONT hFontold = (HFONT)SelectObject(hdc, hfont);

	D3DXCreateText(Device, hdc, "3D", 0.002f, 0.2f, &Text,0,0);

	D3DXCreateFont(Device,
			100,
			100,
			1000,
			0,
			false,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			0,
			"Courier New",
			&Font);

	SelectObject(hdc, hFontold);
	DeleteObject(hfont);
	DeleteDC(hdc);

	//设置光源
// 	D3DXVECTOR3 dir(0.0f, 1.0f, 1.0f);
// 	D3DXCOLOR col = D3DXCOLOR(0,255,0,255);
// 	D3DLIGHT9 light = Initd3d9light(dir, col);
// 	Device->SetLight(0, &light);
// 	Device->LightEnable(0, true);
// 	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
// 	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//定义摄像机
	D3DXVECTOR3  pos(0.0f, 1.0f, -2.5f);
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
}

void Destruct_word()
{
	if (Text)
	{
		Text->Release();
	}

	if (Font)
	{
		Font->Release();
	}
}

D3DLIGHT9 Initd3d9light( D3DXVECTOR3& dir, D3DXCOLOR &color )
{
	D3DLIGHT9 light_;


	return light_;

}

void Construct_rect()
{
	//create vertex buffer
	Device->CreateVertexBuffer(4*sizeof(VertexColor), 
								D3DUSAGE_WRITEONLY, 
								VertexColor::FVFC, 
								D3DPOOL_MANAGED,
								&VB_c, 0);

	//create index buffer
	Device->CreateIndexBuffer(6*sizeof(WORD),
								D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16,
								D3DPOOL_MANAGED,
								&IB_c, 0);

	//insert rect data into the vertex buffer
	VertexColor* v;
	VB_c->Lock(0, 0, (void**)&v, 0);
	v[0] = VertexColor(-1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(255,0,0));
	v[1] = VertexColor(1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(0,255,0));
	v[2] = VertexColor(1.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(0,0,255));
	v[3] = VertexColor(-1.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(255,255,255));

	VB_c->Unlock();

	//insert rect data into index buffer
	WORD* indices = 0;
	IB_c->Lock(0,0, (void**)&indices, 0);
	indices[0] = 0; indices[1] = 3; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 1;
	IB_c->Unlock();

	//set proj matrix
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
								&proj,
								D3DX_PI*0.5f,
								(float)_width/(float)_height,
								1.0f,
								1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);
	Device->SetRenderState(D3DRS_LIGHTING, false);
}

void Destruct_rect()
{
	if (IB_c)
	{
		IB_c->Release();
	}

	if (VB_c)
	{
		VB_c->Release();
	}
}

void Construct_LightModel()
{
	//define color
	D3DCOLORVALUE Blue;
	Blue.r = 0.0f;Blue.g=0.0f;Blue.b=1.0f;Blue.a = 1.0f;
	D3DCOLORVALUE Green;
	Green.r=0.0f; Green.g=1.0f;Green.b=0.0f; Green.a=1.0f;
	D3DCOLORVALUE Black;
	Black.r = 1.0f; Black.g = 1.0f; Black.b = 1.0f; Black.a = 1.0f;
	D3DCOLORVALUE White;
	White.r = 0.0f; White.g = 0.0f; White.b = 0.0f; White.a = 0.0f;
	

	//create vertex buffer
	Device->CreateVertexBuffer(12*sizeof(VertexNor),
								D3DUSAGE_WRITEONLY,
								VertexNor::FVFN,
								D3DPOOL_MANAGED,
								&vb_nor,
								0);

	VertexNor* v;
	vb_nor->Lock(0,0,(void**)&v, 0);
	v[0] = VertexNor(-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[1] = VertexNor(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[2] = VertexNor(0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f);
	v[3] = VertexNor(0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f);
	v[4] = VertexNor(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[5] = VertexNor(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[6] = VertexNor(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[7] = VertexNor(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[8] = VertexNor(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[9] = VertexNor(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[10] = VertexNor(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = VertexNor(-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	vb_nor->Unlock();


	//create material
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = Blue;
	mtrl.Diffuse = Green;
	mtrl.Specular = Green;
	mtrl.Emissive = Black;
	mtrl.Power = 5.0f;
	Device->SetMaterial(&mtrl);

	//open light
	Device->SetRenderState(D3DRS_LIGHTING, false);
	//create light
	D3DLIGHT9 dir;
    ZeroMemory(&dir, sizeof(dir));
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse = Black;
	dir.Specular = HelpFunc_Times(Black, 1.2f);
	dir.Ambient = HelpFunc_Times(Black, 1.2f);
	dir.Direction = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	Device->SetLight(0, &dir);
	Device->LightEnable(0, true);
	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//定义摄像机
	D3DXVECTOR3  pos(1.0f, 1.0f, -3.0f);
	D3DXVECTOR3  target(0.0, 0.0, 0.0);
	D3DXVECTOR3  up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX   V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &V);

	//set proj matrix
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI*0.5f,
		(float)_width/(float)_height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);
	Device->SetRenderState(D3DRS_LIGHTING, false);




}

D3DCOLORVALUE HelpFunc_Times( D3DCOLORVALUE& des, float times )
{
	D3DCOLORVALUE out;
	out.a = des.a*times;
	out.r = des.r*times;
	out.g = des.g*times;
	out.b = des.b*times;
	return out;
}

void Destruct_LightModel()
{
	if (vb_nor)
	{
		vb_nor->Release();
	}
}
