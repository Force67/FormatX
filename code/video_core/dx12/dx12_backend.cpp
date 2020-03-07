
/*
 * FormatX : Video core
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <Windows.h>
#include <VersionHelpers.h>
#include <utl/path.h>

#include "dx12_backend.h"

namespace video_core {
static D3D12CreateDevice_t createDevice{nullptr};
static D3D12GetDebugInterface_t getDebugInterface{nullptr};

dx12Backend::dx12Backend(renderWindow* outWindow) : outputWindow(outWindow), context(this) {}

dx12Backend::~dx12Backend() {
    if (hd3d12) FreeLibrary(hd3d12);
    if (hd3dCompiler) FreeLibrary(hd3dCompiler);
}

bool dx12Backend::create() {
    // dx12 on w7
    if (!IsWindows8OrGreater()) {
        auto path = utl::make_abs_path(L"12on7\\D3D12.DLL");
        hd3d12 = LoadLibraryW(path.c_str());
    } else {
        hd3d12 = LoadLibraryW(L"D3D12.dll");
    }

    hd3dCompiler = LoadLibraryW(L"D3DCompiler_47.dll");

    if (!hd3d12 || !hd3dCompiler) {
        LOG_ERROR("Failed to load dx12 library");
        return false;
    }

    createDevice = (D3D12CreateDevice_t)GetProcAddress(hd3d12, "D3D12CreateDevice");
    getDebugInterface = (D3D12GetDebugInterface_t)GetProcAddress(hd3d12, "D3D12GetDebugInterface");

    if (!createDevice || !getDebugInterface) {
        LOG_ERROR("Failed to find dx12 export");
        return false;
    }

    UINT factoryFlags = 0;

#ifdef _DEBUG
    {
        // enable debug layer
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(getDebugInterface(IID_PPV_ARGS(&debugController)))) {
            debugController->EnableDebugLayer();

            factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#endif

    if (!dxgiContext.init(true, factoryFlags)) 
        return false;

    ComPtr<IDXGIAdapter1> adapter;
    selectDevice(dxgiContext.getFactory().Get(), &adapter);

    if (!adapter) {
        LOG_ERROR("Failed to get a d3d12 compatible device."
                  "Ensure that you have the latest GPU drivers installed");
        return false;
    }

    if (FAILED(createDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device)))) {
        LOG_ERROR("Failed to create a d3d12 device (feature level 11)");
        return false;
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc{};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)))) {
        LOG_ERROR("Failed to create a directx command queue");
        return false;
    }

    if (!dxgiContext.createSwapChain(outputWindow, commandQueue.Get())) 
        return false;

    return true;
}

void dx12Backend::shutdown() {
  
}

void dx12Backend::renderFrame() {

}

void dx12Backend::selectDevice(IDXGIFactory2* factory, IDXGIAdapter1** dxgiAdapter) {

    UINT index = 0;
    ComPtr<IDXGIAdapter1> adapter;

    while (factory->EnumAdapters1(index, &adapter) == S_OK) {
        DXGI_ADAPTER_DESC1 desc;
        if (SUCCEEDED(adapter->GetDesc1(&desc))) {
            if (SUCCEEDED(createDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device),
                                       nullptr))) {

                if (!(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE))
                    break;
            }
        }
    
        ++index;
    }

    *dxgiAdapter = adapter ? adapter.Detach() : nullptr;
}
} // namespace rend