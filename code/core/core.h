#pragma once

/*
 * FormatX : Video core
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <vector>
#include <base.h>
#include <plugin_traits.h>
#include <utl/logger/logger.h>

#include <QApplication>
#include <QFontDatabase>
#include <QIcon>
#include <QObject>

#include <base.h>
#include <core.h>

#include "ui/main_window.h"

namespace core {
enum class result {
    Success,
    ErrorRenderer,
};

class FXCore final : public QApplication {
    Q_OBJECT

public:
    FXCore(int&, char**);
    bool init();

    auto& getPlugins() const {
        return pluginList;
    }

private:
    result initRenderer(video_core::renderWindow&);

    mainWindow window;

    UniquePtr<video_core::renderInterface> renderer;
    std::vector<pluginLoader*> pluginList;
};
}