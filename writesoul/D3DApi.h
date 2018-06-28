#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <memory>
#include "GObject.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

class D3DApi {
	
public:
	bool InitD3d(HWND hwnd);
	void RenderFrame(const std::vector<std::unique_ptr<GObject>>& objs);
	void Clean3d();

private:
	LPDIRECT3D9 d3d = NULL;
	HWND hwnd = NULL;

private:
	void init_graphics(void);

public:
	LPDIRECT3DDEVICE9 d3ddev = NULL;
	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    // the pointer to the vertex buffer
	LPDIRECT3DINDEXBUFFER9 i_buffer = NULL;
	float win_w, win_h;
};


struct CUSTOMVERTEX
{
	FLOAT x, y, z;    // from the D3DFVF_XYZRHW flag
	DWORD color;    // from the D3DFVF_DIFFUSE flag
};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

