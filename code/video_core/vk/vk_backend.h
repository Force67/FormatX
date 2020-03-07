#pragma once

// Copyright (C) 2019-2020 Force67

#if 0
#include <QGuiApplication>
#include <QVulkanInstance>
#include <QVulkanWindow>

#include "../rend.h"

namespace video_core {
class vkBackend final : public renderInterface {
public:
    explicit vkBackend(QWindow*);

    bool create() override;
    void shutdown() override;
    void renderFrame() override;

private:
    QVulkanInstance vkInst;
};
}
#endif