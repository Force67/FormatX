
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "logs.h"

namespace ui {

LogWidget::LogWidget() {
    utl::addLogSink(this);
}

LogWidget::~LogWidget() {
    clear();
    // TODO: remove sink
}

const char* LogWidget::getName() {
    return "LogWidget";
}

void LogWidget::write(const utl::logEntry& entry) {
    auto msg = formatLogEntry(entry).append(1, '\n');

    // TODO: mutex
    log(msg.c_str());
}

void LogWidget::clear() {
    buf.clear();
    lineOffsets.clear();
}

void LogWidget::log(const char* fmt, ...) {
    int old_size = buf.size();
    va_list args;
    va_start(args, fmt);
    buf.appendfv(fmt, args);
    va_end(args);
    for (int new_size = buf.size(); old_size < new_size; old_size++)
        if (buf[old_size] == '\n')
            lineOffsets.push_back(old_size);
    scrollToBottom = true;
}

void LogWidget::log(char c) {
    ImVector<char>& chars = buf.Buf;
    if (c == '\n')
        lineOffsets.push_back(buf.size());
    chars.push_back(c);
    scrollToBottom = true;
}

void LogWidget::draw(const char* title, bool& open) {
    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin(title, &open);

    if (ImGui::Button("Clear"))
        clear();

    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    filter.Draw("Filter", -100.0f);
    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (copy)
        ImGui::LogToClipboard();

    if (filter.IsActive()) {
        const char* buf_begin = buf.begin();
        const char* line = buf_begin;
        for (int line_no = 0; line != NULL; line_no++) {
            const char* line_end =
                (line_no < lineOffsets.Size) ? buf_begin + lineOffsets[line_no] : NULL;
            if (filter.PassFilter(line, line_end))
                ImGui::TextUnformatted(line, line_end);
            line = line_end && line_end[1] ? line_end + 1 : NULL;
        }
    } else {
        ImGui::TextUnformatted(buf.begin());
    }

    if (scrollToBottom)
        ImGui::SetScrollHere(1.0f);

    scrollToBottom = false;
    ImGui::EndChild();
    ImGui::End();
}

}