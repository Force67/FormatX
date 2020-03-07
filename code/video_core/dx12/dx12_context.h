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

namespace video_core {
// for ComPtr
using namespace Microsoft::WRL;

class dx12Backend;

class dx12Context {
public:
    explicit dx12Context(dx12Backend*);

    bool create(const dxgi&);

private:
    dx12Backend* backend;

    ComPtr<IDXGISwapChain3> swapChain;
    ComPtr<ID3D12DescriptorHeap> rtvHeap;

    static const UINT FrameCount = 2;
};
}