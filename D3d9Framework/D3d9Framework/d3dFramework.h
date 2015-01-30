#include<windows.h>
#include "d3dx9.h"
bool d3d9Initial(HINSTANCE hInstance, 
					WNDPROC wndProc, 
					int width , 
					int height,
					bool fullscreen, 
					D3DDEVTYPE deviceType,
					IDirect3DDevice9** device);

HWND d3d9CreateWindow(HINSTANCE hInstance, 
						WNDPROC wndProc, 
						int width, 
						int height, 
						bool fullscreen);

int d3d9CheckHardwareVP(IDirect3D9* d3d9, D3DDEVTYPE deviceType);

D3DPRESENT_PARAMETERS d3d9CreatePP(HWND hwnd, 
									int width, 
									int heigh, 
									bool fullscreen);

bool d3d9CreateDevice(IDirect3D9* d3d9, 
						D3DDEVTYPE deviceType, 
						HWND hwnd, 
						int vp, 
						D3DPRESENT_PARAMETERS d3dpp,
						IDirect3DDevice9** device);

int d3d9MessageLoop(bool (* p_Display)(float time, IDirect3DDevice9* Device_), IDirect3DDevice9* Device_);

