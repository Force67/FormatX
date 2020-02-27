#pragma once

// Copyright (C) 2019-2020 Force67

#include <QGuiApplication>
#include <QVulkanInstance>
#include <QVulkanWindow>

#include "../rend.h"

#include <wrl.h>

#include <d3d12.h>
#include <dxgi1_4.h>

#include "dx12_context.h"

namespace rend {
// for ComPtr
using namespace Microsoft::WRL;

class dx12Backend final : public renderInterface {
public:
    explicit dx12Backend(QWindow* parent);

    bool create() override;
    void shutdown() override;
    void renderFrame() override;

    inline auto& getFactory() { return factory; }
    inline auto& getQueue() { return commandQueue; }
    inline auto& getDevice() { return device; }

    inline QWindow* getWindow() const { return outputWindow; }

private:
    void selectDevice(IDXGIFactory2*, IDXGIAdapter1**);

    decltype(&::D3D12CreateDevice) createDevice;

    QWindow* outputWindow = nullptr;
    ComPtr<IDXGIFactory4> factory;
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Device> device;
    ComPtr<ID3D12CommandQueue> commandQueue;

    std::unique_ptr<dx12Context> context;
};
}