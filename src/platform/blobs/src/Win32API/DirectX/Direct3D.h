#ifndef BLOBS_SRC_WIN32API_DIRECTX_DIRECT3D_H
#define BLOBS_SRC_WIN32API_DIRECTX_DIRECT3D_H

#include "Win32API/WinUtils.h"
#include "Win32API/Handle.h"

struct IDirect3D9;
extern "C" IDirect3D9 *Direct3DCreate9(uint32_t sdk_version);
extern "C" int D3DXLoadSurfaceFromMemory(); // TODO: stubbed

#endif //BLOBS_SRC_WIN32API_DIRECTX_DIRECT3D_H
