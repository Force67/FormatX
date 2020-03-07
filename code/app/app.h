#pragma once

// Copyright (C) 2019-2020 Force67

#include <QApplication>
#include <QFontDatabase>
#include <QIcon>
#include <QObject>

#include <plugin_traits.h>
#include "gui/main_window.h"
#include "video_core.h"

class fmtApp : public QApplication {
    Q_OBJECT
public:
    using pluginList = std::vector<pluginLoader*>;

    fmtApp(int&, char**);

    bool loadPlugins();
    bool createViewport();

    pluginList& getPlugins() {
        return plugins;
    }

private:
    pluginList plugins;
    std::unique_ptr<mainWindow> window;
    std::unique_ptr<video_core::renderInterface> renderer;
};