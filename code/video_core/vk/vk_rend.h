#pragma once

// Copyright (C) 2019-2020 Force67

#if 0
#include <QGuiApplication>
#include <QVulkanInstance>
#include <QVulkanWindow>

#include "../rend.h"

namespace video_core {
class vkBackend : public renderInterface, public QVulkanWindow {
public:
    vkBackend(QWindow* parent) : QVulkanWindow(parent) {}

    bool create() override;
    void shutdown() override;
    void renderFrame() override;

    inline QVulkanInstance& vkInstance() {
        return vkInst;
    }

private:
    QVulkanWindowRenderer* createRenderer() override;
    QVulkanInstance vkInst;
};
}
#endif