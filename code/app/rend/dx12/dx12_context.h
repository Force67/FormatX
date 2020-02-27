#pragma once

// Copyright (C) 2019-2020 Force67

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_4.h>

class QWindow;

namespace rend {
// for ComPtr
using namespace Microsoft::WRL;

class dx12Backend;

class dx12Context {
public:
    explicit dx12Context(dx12Backend*);

    bool create();

private:
    dx12Backend* backend;

    ComPtr<IDXGISwapChain3> swapChain;
    ComPtr<ID3D12DescriptorHeap> rtvHeap;

    static const UINT FrameCount = 2;
};
}