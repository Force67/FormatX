#pragma once

// Copyright (C) 2019-2020 Force67

#include <QWindow>
#include <video_core.h>

class renderWindow : public video_core::renderWindow, public QWindow {
public:
    renderWindow();

    void* getHandle() override;
    void getDimensions(u32& width, u32& height) override;

private:
};