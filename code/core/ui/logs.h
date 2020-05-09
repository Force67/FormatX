#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <imgui.h>
#include <logger/logger.h>

namespace ui {

class LogWidget final : public utl::logBase {
public:
    LogWidget();
    ~LogWidget();

    void log(const char* fmt, ...);
    void log(char);

    void draw(const char* title, bool& open);
    void clear();

private:
    const char* getName() override;
    void write(const utl::logEntry&) override;

private:
    ImGuiTextBuffer buf;
    ImGuiTextFilter filter;
    ImVector<int> lineOffsets;
    bool scrollToBottom = false;
};
}