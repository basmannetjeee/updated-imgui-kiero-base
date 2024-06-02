#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "3rd_party/kiero/kiero.h"
#include "3rd_party/imgui/imgui.h"
#include "3rd_party/imgui/imgui_impl_win32.h"
#include "3rd_party/imgui/imgui_impl_dx11.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;