#pragma once
#include "D3DApi.h"
#include "utils.h"

//D3DApi::D3DApi() {
//	log("D3d Create\n");
//}
void InitSquare(D3DApi* api);
void InitRect(D3DApi* api);

// 画一个带颜色的Panel
void MoveFrame(LPDIRECT3DDEVICE9);
void DrawRect(D3DApi *api);

void InitPanel(D3DApi *api);
void DrawOnePanel(D3DApi *api, GObject *g);

bool D3DApi::InitD3d(HWND hwnd) {
	this->hwnd = hwnd;
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
	d3dpp.BackBufferWidth = clientRect.right;    // set the width of the buffer
	d3dpp.BackBufferHeight = clientRect.bottom;    // set the height of the buffer
	d3dpp.EnableAutoDepthStencil = TRUE;    // automatically run the z-buffer for us
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	HRESULT hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, 
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if (!SUCCEEDED(hr)) {
		log("初始化d3d失败！\n");
		return false;
	}
	init_graphics();
	d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE); 
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);    // both sides of the triangles
	d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	return true;
}

void D3DApi::RenderFrame(const std::vector<std::unique_ptr<GObject>>& objs) {
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->BeginScene();

	// directX默认是左手坐标系
	// select which vertex format we are using
	d3ddev->SetFVF(CUSTOMFVF);
	D3DXMATRIX matView;    // the view transform matrix
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(0.0f, 0.f, -5.f),    // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.f),      // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));    // the up direction
	d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	win_w = clientRect.right;
	win_h = clientRect.bottom;
														// set the projection transform
	D3DXMATRIX matProjection;    // the projection transform matrix
	D3DXMatrixOrthoLH(&matProjection,
		//D3DXToRadian(45),    // the horizontal field of view
		win_w, win_h, // aspect ratio
		0.f,   // the near view-plane
		100.0f);    // the far view-plane
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection); // set the projection

	for (auto const & obj : objs) {
		DrawOnePanel(this, obj.get());
	}
	//MoveFrame(d3ddev);
	//DrawRect(this);
	
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void D3DApi::Clean3d() {
	i_buffer->Release();
	v_buffer->Release();
	d3ddev->Release();
	d3d->Release();
}

// this is the function that puts the 3D models into video RAM
void D3DApi::init_graphics(void)
{
	//InitSquare(this);
	InitRect(this);
	//InitPanel(this);
}

void MoveFrame(LPDIRECT3DDEVICE9 d3ddev) {
	D3DXMATRIX matView;    // the view transform matrix
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(0.0f, 8.0f, 25.0f),    // the camera position
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),      // the look-at position
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));    // the up direction
	d3ddev->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView 

												   // set the projection transform
	D3DXMATRIX matProjection;    // the projection transform matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),    // the horizontal field of view
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
		1.0f,   // the near view-plane
		100.0f);    // the far view-plane
	d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection); // set the projection

															// set the world transform
	static float index = 0.0f; index += 0.03f; // an ever-increasing float value
	D3DXMATRIX matRotateY;    // a matrix to store the rotation for each triangle
	D3DXMatrixRotationY(&matRotateY, index);    // the rotation matrix
	d3ddev->SetTransform(D3DTS_WORLD, &(matRotateY));    // set the world transform

	// draw the Hypercraft
	d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

void InitSquare(D3DApi * api) {
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		{ -3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(0, 0, 255), },
	{ 3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(0, 255, 0), },
	{ -3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(255, 0, 0), },
	{ 3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(0, 255, 255), },
	{ -3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(0, 0, 255), },
	{ 3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(255, 0, 0), },
	{ -3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 255, 0), },
	{ 3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 255, 255), },
	};

	// create a vertex buffer interface called v_buffer
	api->d3ddev->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&api->v_buffer,
		NULL);

	VOID* pVoid;    // a void pointer

					// lock v_buffer and load the vertices into it
	api->v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	api->v_buffer->Unlock();

	// create the indices using an int array
	short indices[] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 0, 6,    // side 2
		6, 0, 2,
		7, 5, 6,    // side 3
		6, 5, 4,
		3, 1, 7,    // side 4
		7, 1, 5,
		4, 5, 0,    // side 5
		0, 5, 1,
		3, 7, 2,    // side 6
		2, 7, 6,
	};

	// create an index buffer interface called i_buffer
	api->d3ddev->CreateIndexBuffer(36 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&api->i_buffer,
		NULL);

	// lock i_buffer and load the indices into it
	api->i_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	api->i_buffer->Unlock();
}

void InitRect(D3DApi* api) {
	CUSTOMVERTEX vertices[] = {
		{ -SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.f, D3DCOLOR_XRGB(255, 255, 0), },
		{ 0.f, SCREEN_HEIGHT / 2, 0.f, D3DCOLOR_XRGB(255, 255, 0), },
		{ 0.f, 0.f, 0.f, D3DCOLOR_XRGB(255, 255, 0), },
		{ -SCREEN_WIDTH / 2, 0.f, 0.f, D3DCOLOR_XRGB(255, 255, 0), },
	};
	api->d3ddev->CreateVertexBuffer(4 * sizeof(vertices),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&api->v_buffer,
		NULL);

	VOID* pVoid;    // a void pointer
	// lock v_buffer and load the vertices into it
	api->v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	api->v_buffer->Unlock();

	short indices[] = {
		0, 1, 2,
		0, 3, 2,
	};

	api->d3ddev->CreateIndexBuffer(6 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&api->i_buffer,
		NULL);

	api->i_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	api->i_buffer->Unlock();
}

void InitPanel(D3DApi *api) {
	short indices[] = {
		0, 1, 2,
		0, 3, 2,
	};

	api->d3ddev->CreateIndexBuffer(6 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&api->i_buffer,
		NULL);

	VOID* pVoid;
	api->i_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	api->i_buffer->Unlock();
}

// todo，添加title
void DrawOnePanel(D3DApi *api, GObject *g) {
	auto color = D3DCOLOR_XRGB(g->color.r, g->color.g, g->color.b);
	float x1 = g->gx - api->win_w / 2;
	float y1 = api->win_h / 2 - g->gy;
	float x2 = x1 + g->gw;
	float y2 = y1 - g->gh;
	//log("x=%f y=%f w=%f h=%f\n", x1, y1, g->w, g->h);
	CUSTOMVERTEX vertices[] = {
		{ x1, y1, -3.0f, color, },
		{ x2, y1, -3.0f, color, },
		{ x2, y2, -3.0f, color, },
		{ x1, y2, -3.0f, color, },
	};
	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;
	api->d3ddev->CreateVertexBuffer(4 * sizeof(vertices),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid;    // a void pointer
					// lock v_buffer and load the vertices into it
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();

	api->d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	api->d3ddev->SetIndices(api->i_buffer);
	api->d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void DrawRect(D3DApi *api) {
	api->d3ddev->SetStreamSource(0, api->v_buffer, 0, sizeof(CUSTOMVERTEX));
	api->d3ddev->SetIndices(api->i_buffer);
	api->d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}