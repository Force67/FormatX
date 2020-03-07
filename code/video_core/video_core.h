#pragma once

/*
 * FormatX : Video core
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <utl/logger/logger.h>

class QWindow;

namespace video_core {
enum class backendKind { 
    null, 
    vulkan, 
    dx12, 
    opengl 
};

class renderWindow {
public:
    virtual void* getHandle() = 0;
    virtual void getDimensions(u32& width, u32& height) = 0;
};

class renderInterface {
public:
    virtual bool create() = 0;
    virtual void shutdown() = 0;
    virtual void renderFrame() = 0;

    virtual bool requestScreenshot() {
        return false;
    }
};

UniquePtr<renderInterface> createRenderer(renderWindow*, backendKind);
}