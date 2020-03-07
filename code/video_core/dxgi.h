#pragma once

/*
 * FormatX : Video core
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <wrl.h>
#include <dxgi1_6.h>
#include <Windows.h>

#include "video_core.h"

namespace video_core {
// for ComPtr
using namespace Microsoft::WRL;

using CreateDXGIFactory1_t = HRESULT(WINAPI*)(const IID&, void**);
using CreateDXGIFactory2_t = HRESULT(WINAPI*)(UINT, const IID&, void**);
using DXGIGetDebugInterface_t = HRESULT(WINAPI*)(const IID&, void**);
using DXGIGetDebugInterface1_t = HRESULT(WINAPI*)(UINT, const IID&, void**);

class dxgi {
public:
    dxgi();
    ~dxgi();

    bool init(bool forceNewDxgi, UINT factoryFlags);
    bool createSwapChain(renderWindow*, IUnknown* device);
    HRESULT resize(u32 newWidth, u32 newHeight, const u32 *nodeMask, 
        IUnknown* const* presentQueue = nullptr /*Only supply from d3d12 context*/);

    inline auto& getFactory() { return factory; }
    inline auto& getSwapchain() { return swapChain; }

private:
    void selectDevice(IDXGIAdapter1**);

    HMODULE hdxgi = nullptr;
    HMODULE hdxgiDebug = nullptr;
    DXGI_SWAP_CHAIN_DESC1 desc{};

    ComPtr<IDXGIFactory5> factory;
    ComPtr<IDXGISwapChain4> swapChain;
};
} // namespace video_core