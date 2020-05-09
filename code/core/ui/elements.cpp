
/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <imgui.h>
#include "elements.h"

namespace ui {

#if 0
bool MessageBox::Init(const char* title, const char* icon, const char* text, const char** captions,
        bool show_checkbox /* = false */) {

    m_Title = title;
    m_Icon = icon;
    m_Text = text;
    m_Captions = captions;
    m_ShowCheckbox = show_checkbox;
    m_DontAskAgain = false;
    m_Selected = 0;
    return true;
}

int MessageBox::Draw() {
    int index = 0;

    if (ImGui::BeginPopupModal(m_Title, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (m_DontAskAgain && m_Selected != 0) {
            ImGui::CloseCurrentPopup();
            index = m_Selected;
        } else {
            if (m_Icon != NULL) {
                ImVec2 size = ImGui::CalcTextSize(m_Icon);
                ImVec2 pos = ImGui::GetCursorPos();
                float save_y = pos.y;
                pos.y += size.y;

                ImGui::SetCursorPos(pos);
                ImGui::Text("%s", m_Icon);

                pos.x += size.x + pos.x;
                pos.y = save_y;

                ImGui::SetCursorPos(pos);
                ImGui::TextWrapped("%s", m_Text);
            } else {
                ImGui::TextWrapped("%s", m_Text);
            }

            ImGui::Separator();

            if (m_ShowCheckbox) {
                ImGui::Checkbox("Don't ask me again", &m_DontAskAgain);
            }

            ImVec2 size = ImVec2(50.0f, 0.0f);
            int count;

            for (count = 0; m_Captions[count] != NULL; count++) {
                if (ImGui::Button(m_Captions[count], size)) {
                    index = count + 1;
                    ImGui::CloseCurrentPopup();
                    break;
                }

                ImGui::SameLine();
            }

            size = ImVec2((4 - count) * 50.0f, 1.0f);
            ImGui::Dummy(size);

            if (m_DontAskAgain) {
                m_Selected = index;
            }
        }

        ImGui::EndPopup();
    }

    return index;
}

void MessageBox::Open() {
    ImGui::OpenPopup(m_Title);
}
#endif
}