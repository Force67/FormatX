#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

namespace ui {

class Dialog {
public:
    Dialog();

private:

};

#if 0
class MessageBox {
public:
    enum ActionType { Save, Discard, Cancel };

    inline MessageBox(ActionType actions, const char* title) : actions(actions), m_Title(title) {}

    bool Init(const char* title, const char* icon, const char* text, const char** captions,
              bool show_checkbox = false);
    int Draw();
    void Open();

private:
    ActionType actions;
    const char* title;

    const char* m_Title;
    const char* m_Icon;
    const char* m_Text;
    const char** m_Captions;
    bool m_ShowCheckbox;
    int m_Selected;
};
#endif
}