
// Copyright (C) 2019-2020 Force67

#include "render_window.h"

renderWindow::renderWindow() {

}

// returns hwnd
void* renderWindow::getHandle() {
    return reinterpret_cast<void*>(QWindow::winId());
}

void renderWindow::getDimensions(u32& width, u32& height) {
    width = QWindow::width();
    height = QWindow::height();
}

