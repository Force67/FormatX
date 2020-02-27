#pragma once

// Copyright (C) 2019-2020 Force67

#include <memory>
#include <utl/logger/logger.h>

class QWindow;

namespace rend {
enum class backendKind { 
    null, 
    vulkan, 
    dx12, 
    opengl 
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

std::unique_ptr<renderInterface> createRenderer(QWindow*, backendKind);
}