#pragma once

#include <Windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define WIDTH 1536
#define HEIGHT 864

using namespace std;
using namespace DirectX;

bool fequal(float, float);

#define PI 3.141592f

#define RtoD 180.0f / PI
#define DtoR PI / 180.0f