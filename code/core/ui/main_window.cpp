
// Copyright (C) 2019-2020 Force67

#include <QApplication>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QDragMoveEvent>
#include <QFileDialog>
#include <QLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QVBoxLayout>

#include "license_text.h"
#include "load_file_dialog.h"
#include "main_window.h"

class fileItemTab : public QWidget {
public:
    fileItemTab(QWidget* parent, QWidget* dock, bool canRender = false) : QWidget(parent) {
        QVBoxLayout* mainLayout = new QVBoxLayout;
        mainLayout->addWidget(dock);
        setLayout(mainLayout);
    }
};

mainWindow::mainWindow(core::FXCore& app) : 
    app(app), QMainWindow(nullptr), tabBar(this) {
    setAcceptDrops(true);
}

void mainWindow::init() {
    ui.setupUi(this);
    setWindowTitle("FormatX | " FX_BRANCH "@" FX_COMMITHASH);

    connect(ui.openFileAct, &QAction::triggered, this, &mainWindow::onOpenFile);
    connect(ui.aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui.aboutFBXAct, &QAction::triggered,
            [&] { QMessageBox::about(this, "About Autodesk\302\256 FBX\302\256", licenseText); });

    auto* dock = new QDockWidget(tr("Render window | %1").arg("FIXME"), this);
    dock->setFeatures(dock->features() & ~QDockWidget::DockWidgetFeature::DockWidgetClosable);

    // wrap the render window within a widget
    rendChild = QWidget::createWindowContainer(&renderView, dock);
    dock->setWidget(rendChild);

    tabBar.addTab(new fileItemTab(&tabBar, dock), "testfile.dat");
    setCentralWidget(&tabBar);

    // ensure geometry is set properly
    updateChild();
}

// stupid hack since i cant get some *decent* looking alignment working using qt
// creator
// also: menuBar dimensions are wrong
// TODO: please someone fix this
void mainWindow::updateChild() {
    auto geom = this->geometry();

    const auto magicOffset = !fullmodeEditor ? 76 : 0; /*size of menubar returns bullshit*/
    rendChild->setGeometry(QRect(0, magicOffset, geom.width(), geom.height() - magicOffset));
}

void mainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);

    if (rendChild)
        updateChild();
}

void mainWindow::keyPressEvent(QKeyEvent* event) {
    // toggle tool bar
    if (event->key() == Qt::Key_F11) {
        fullmodeEditor = !fullmodeEditor;

        if (fullmodeEditor) {
            ui.menuBar->hide();
            setWindowState(Qt::WindowFullScreen);
        } else {
            setWindowState(Qt::WindowMaximized);
            ui.menuBar->show();
        }
    }
}

void mainWindow::onOpenFile() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select file to load"), "", tr("*"),
                                                    Q_NULLPTR, QFileDialog::DontResolveSymlinks);

    if (filePath != nullptr) {
        auto qs = QFileInfo(filePath).canonicalFilePath();

        loadFileDialog diag(this);
        if (diag.load(app, qs))
            diag.exec();
    }
}

static bool isValidFile(const QMimeData& md) {
    auto& list = md.urls();

    // only allow one file
    if (list.size() > 1 || !list.size())
        return false;

    auto& url = list[0];
    const auto path = url.toLocalFile();
    const QFileInfo info = path;

    if (info.isDir())
        return false;

    return true;
}

void mainWindow::dragEnterEvent(QDragEnterEvent* event) {
    if (isValidFile(*event->mimeData()))
        event->accept();
}

void mainWindow::dragMoveEvent(QDragMoveEvent* event) {
    if (isValidFile(*event->mimeData()))
        event->accept();
}

void mainWindow::dragLeaveEvent(QDragLeaveEvent* event) {
    event->accept();
}

void mainWindow::dropEvent(QDropEvent* event) {
    auto& md = *event->mimeData();

    if (isValidFile(md)) {
        const auto path = md.urls()[0].toLocalFile();

        loadFileDialog diag(this);
        if (diag.load(app, path))
            diag.exec();
    }
}