#pragma once

// Copyright (C) 2019-2020 Force67

#include <QDesktopWidget>
#include <QDockWidget>
#include <QMainWindow>

namespace Ui {
class main_window;
}

class fmtApp;

class mainWindow : public QMainWindow {
    Q_OBJECT

public:
    mainWindow(fmtApp&);

    void init(QWindow*, const char*);

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

    fmtApp& app;
    std::unique_ptr<Ui::main_window> ui;
    std::unique_ptr<QWidget> rendChild;
    std::unique_ptr<QTabWidget> tabBar;

    bool fullmodeEditor = false;
};