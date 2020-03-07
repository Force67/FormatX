
/*
 * FormatX : Video core
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <dxgidebug.h>

#include "dxgi.h"

namespace video_core {
static CreateDXGIFactory1_t createFactory1;
static CreateDXGIFactory2_t createFactory2;
static DXGIGetDebugInterface_t getDebugInterface;
static DXGIGetDebugInterface1_t getDebugInterface1;
static constexpr UINT FrameCount = 2;

static constexpr GUID IID_IDXGIFactory4 = {
    0x1bc6ea02, 0xef36, 0x464f, {0xbf, 0x0c, 0x21, 0xca, 0x39, 0xe5, 0x16, 0x8a}};

dxgi::dxgi() {}

dxgi::~dxgi() {
    // no need to nullify the handles, they are not usable anyway
    if (hdxgi)
        FreeLibrary(hdxgi);
    if (hdxgiDebug)
        FreeLibrary(hdxgiDebug);
}

bool dxgi::init(bool forceNewDxgi, UINT factoryFlags) {

    if (!hdxgi)
        hdxgi = LoadLibraryW(L"dxgi.dll");
    if (!hdxgi) {
        LOG_ERROR("Failed to load dxgi library");
        return false;
    }

    if (!hdxgiDebug)
        hdxgiDebug = LoadLibraryW(L"dxgidebug.dll");
    if (hdxgiDebug) {
        getDebugInterface =
            (DXGIGetDebugInterface_t)GetProcAddress(hdxgiDebug, "DXGIGetDebugInterface");
        getDebugInterface1 =
            (DXGIGetDebugInterface1_t)GetProcAddress(hdxgiDebug, "DXGIGetDebugInterface1");

        if (!getDebugInterface && !getDebugInterface1) {
            FreeLibrary(hdxgiDebug);
            hdxgiDebug = nullptr;
        }
    } else
        LOG_WARNING("Failed to load dxgi debug library");

    createFactory2 = (CreateDXGIFactory2_t)GetProcAddress(hdxgi, "CreateDXGIFactory2");
    if (!createFactory2) {
        createFactory1 = (CreateDXGIFactory1_t)GetProcAddress(hdxgi, "CreateDXGIFactory1");
    }

    if (!createFactory1 && !createFactory2) {
        LOG_ERROR("Unable to find CreateDXGIFactory export in dxgi library");
        return false;
    }

    // create the right factory
    HRESULT hr = createFactory2 ? createFactory2(factoryFlags, IID_PPV_ARGS(&factory))
                                : createFactory1(IID_PPV_ARGS(&factory));

    if (FAILED(hr)) {
        LOG_ERROR("Unable to create dxgi v5 factory");
        return false;
    }

    return true;
}

bool dxgi::createSwapChain(renderWindow* window, IUnknown* device) {
    uint32_t width, height;
    window->getDimensions(width, height);
    
    if (width == 0 || height == 0)
        LOG_WARNING("attempting to create an invisible window");

    BOOL supportsTearing = 0;

    // will crash on w7
    if (createFactory2) {
        factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &supportsTearing,
                                     sizeof(supportsTearing));
    }

    // describe the swapchain
    desc.BufferCount = FrameCount;
    desc.Width = width;
    desc.Height = height;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.SampleDesc.Count = 1;
    desc.Flags = supportsTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

    {
        ComPtr<IDXGISwapChain1> swapChain1;
        if (FAILED(factory->CreateSwapChainForHwnd(device, static_cast<HWND>(window->getHandle()),
                                                   &desc, nullptr, nullptr, &swapChain1))) {
            LOG_ERROR("Failed to create a DXGI swapchain");
            return false;
        }

        if (FAILED(swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain)))) {
            LOG_ERROR("Failed to get a version 3 DXGI swapchain interface");
            return false;
        }
    }

    return true;
}

HRESULT dxgi::resize(u32 newWidth, u32 newHeight, const u32 *nodeMask, IUnknown * const *presentQueue) {
    desc.Width = newWidth;
    desc.Height = newHeight;

    HRESULT hr = presentQueue
                     ? swapChain->ResizeBuffers1(desc.BufferCount, newWidth, newHeight, desc.Format,
                                                 desc.Flags, nodeMask, presentQueue)
                     : swapChain->ResizeBuffers(desc.BufferCount, newWidth, newHeight, desc.Format,
                                                desc.Flags);

    return hr;
}

void dxgi::selectDevice(IDXGIAdapter1** dxgiAdapter) {}

} // namespace video_core