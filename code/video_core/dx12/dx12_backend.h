#pragma once

/*
 * FormatX : Video core
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */


#include <d3d12.h>
#include "dxgi.h"
#include "dx12_context.h"

namespace video_core {
// for ComPtr
using namespace Microsoft::WRL;

using D3D12CreateDevice_t = decltype(&::D3D12CreateDevice);
using D3D12GetDebugInterface_t = decltype(&::D3D12GetDebugInterface);

class dx12Backend final : public renderInterface {
public:
    explicit dx12Backend(renderWindow* parent);
    ~dx12Backend();

    bool create() override;
    void shutdown() override;
    void renderFrame() override;

    inline auto& getQueue() { return commandQueue; }
    inline auto& getDevice() { return device; }

    inline renderWindow* getWindow() const {
        return outputWindow;
    }
    inline dx12Context& getContext() { return context; }

private:
    void selectDevice(IDXGIFactory2*, IDXGIAdapter1**);

    HMODULE hd3d12 = nullptr;
    HMODULE hd3dCompiler = nullptr;
    renderWindow* outputWindow = nullptr;

    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12CommandQueue> commandQueue;

    dxgi dxgiContext;
    dx12Context context;
};
}