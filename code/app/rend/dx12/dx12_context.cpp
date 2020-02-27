
// Copyright (C) 2019-2020 Force67

#include "dx12_backend.h"
#include "dx12_context.h"

namespace rend {
dx12Context::dx12Context(dx12Backend* backend) : backend(backend) {}

bool dx12Context::create() {
    auto factory = backend->getFactory();
    auto device = backend->getDevice();

    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.BufferCount = FrameCount;
    desc.Width = backend->getWindow()->width();
    desc.Height = backend->getWindow()->height();
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.SampleDesc.Count = 1;
    {
        ComPtr<IDXGISwapChain1> swapChain1;
        if (FAILED(factory->CreateSwapChainForHwnd(backend->getQueue().Get(),
                                                   (HWND)backend->getWindow()->winId(), &desc,
                                                   nullptr, nullptr, &swapChain1))) {
            LOG_ERROR("Failed to create dx12 DXGI swapchain");
            return false;
        }

        if (FAILED(swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain)))) {
            LOG_ERROR("Failed to get a version 3 DXGI swapchain interface");
            return false;
        }
    }

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

    //m_rtvDescriptorSize =
    //    m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}
} // namespace rend