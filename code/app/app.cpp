
// Copyright (C) 2019-2020 Force67

#ifdef _WIN32
#include <Windows.h>
#endif

#include <QDirIterator>
#include <QMessageBox>
#include <QWindow>

#include <utl/File.h>
#include <utl/path.h>

#include "app.h"

#include <video_core.h>

fmtApp::fmtApp(int& argc, char** argv) : QApplication(argc, argv),
    window(*this) {
    setApplicationName("FormatX");
    setApplicationVersion("1.0");
}

bool fmtApp::init() {
    window.init();
    window.show();

    const auto state = core.init(window.getRenderWindow());
    if (state != core::result::success) {
        LOG_ERROR("Failed to initialize core (Result {})", static_cast<int>(state));
        return false;
    }

    return true;
}

bool fmtApp::present() {
    //window.show();
    return true;
}