#pragma once

/*
 * FormatX
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "window.h"
#include "imgui_driver.h"

namespace ui {

class FXEditor final : public ImguiDriver {
public:
    explicit FXEditor(FXWindow&);

    void update();
};
}