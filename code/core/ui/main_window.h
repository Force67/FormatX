#pragma once

// Copyright (C) 2019-2020 Force67

#include <QDesktopWidget>
#include <QDockWidget>
#include <QMainWindow>

#include "render_window.h"
#include "qtgen/ui_main_window.h"

namespace core {
class FXCore;
}

class mainWindow : public QMainWindow {
    Q_OBJECT

public:
    mainWindow(core::FXCore&);
    void init();

    renderWindow& getRenderWindow() { return renderView;}
private:
    /*callbacks*/
    void onOpenFile();

    // drag'n drop
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    void updateChild();

    Ui::main_window ui;
    renderWindow renderView;
    QWidget *rendChild;
    QTabWidget tabBar;
    core::FXCore& app;

    bool fullmodeEditor = false;
};