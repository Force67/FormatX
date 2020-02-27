
// Copyright (C) 2019-2020 Force67

#include <Windows.h>
#include <VersionHelpers.h>

#include "dx12_backend.h"

namespace rend {
dx12Backend::dx12Backend(QWindow* outWindow) : outputWindow(outputWindow) {
    context = std::make_unique<dx12Context>(this);
}

bool dx12Backend::create() {
    // backwards compat feature
    HMODULE hd3d12 =
        !IsWindows8OrGreater() ? LoadLibraryW(L"12on7\\D3D12.DLL") : LoadLibraryW(L"D3D12.DLL");

    HMODULE hdxgi = LoadLibraryW(L"dxgi.dll");
    HMODULE hd3dCompiler = LoadLibraryW(L"D3DCompiler_47.dll");

    if (!hd3d12 || !hdxgi || !hd3dCompiler) {
        LOG_ERROR("Failed to load dx12 library");
        return false;
    }

    createDevice = (decltype(&::D3D12CreateDevice))GetProcAddress(hd3d12, "D3D12CreateDevice");
    auto getDebugInterface =
        (decltype(&::D3D12GetDebugInterface))GetProcAddress(hd3d12, "D3D12GetDebugInterface");
    auto createFactory =
        (decltype(&::CreateDXGIFactory2))GetProcAddress(hdxgi, "CreateDXGIFactory2");

    if (!createDevice || !getDebugInterface || !createFactory) {
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

    if (FAILED(createFactory(factoryFlags, IID_PPV_ARGS(&factory)))) {
        __debugbreak();
        return false;
    }

    // select a hardware device (gpu)
    ComPtr<IDXGIAdapter1> hwAdapter;
    selectDevice(factory.Get(), &hwAdapter);

    if (FAILED(createDevice(hwAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)))) {
        __debugbreak();
        return false;
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc{};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)))) {
        __debugbreak();
        return false;
    }
}

void dx12Backend::shutdown() {
  
}

void dx12Backend::renderFrame() {

}

void dx12Backend::selectDevice(IDXGIFactory2* factory, IDXGIAdapter1** dxgiAdapter) {
    ComPtr<IDXGIAdapter1> adapter;
    *dxgiAdapter = nullptr;

    for (UINT adapterIndex = 0;
         DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex) {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        // no virtual device
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            continue;

        // Check to see if the adapter supports Direct3D 12, but don't create the
        // actual device yet.
        if (SUCCEEDED(createDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device),
                                   nullptr))) {
            break;
        }
    }

    *dxgiAdapter = adapter.Detach();
}
} // namespace rend