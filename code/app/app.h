#pragma once

// Copyright (C) 2019-2020 Force67

#include <QApplication>
#include <QFontDatabase>
#include <QIcon>
#include <QObject>

#include <core.h>

#include <plugin_traits.h>
#include "ui/main_window.h"
#include "video_core.h"

class fmtApp : public QApplication {
    Q_OBJECT
public:
    fmtApp(int&, char**);

    bool init();
    bool present();

private:
    mainWindow window;
    core::fxcore core;
};