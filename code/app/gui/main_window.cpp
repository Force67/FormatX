
// Copyright (C) 2019-2020 Force67

#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QDesktopWidget>
#include <QMimeData>
#include <QDragMoveEvent>
#include <QLayout>

#include "app.h"
#include "main_window.h"
#include "license_text.h"
#include "load_file_dialog.h"

#include "qtgen/ui_main_window.h"

static QRect g_WindowSize{ 0,0, 720, 1080 };

mainWindow::mainWindow(fmtApp &app) :
	app(app),
	QMainWindow(nullptr),
	ui(new Ui::main_window())
{
	setAcceptDrops(true);
}

mainWindow::~mainWindow()
{
	if (rendChild)
		delete rendChild;

	delete ui;
}

void mainWindow::init(QWindow *subWindow)
{
	ui->setupUi(this);

	connect(ui->openFileAct, &QAction::triggered, this, &mainWindow::onOpenFile);
	connect(ui->aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
	connect(ui->aboutFBXAct, &QAction::triggered, [&] {QMessageBox::about(this, "About Autodesk\302\256 FBX\302\256",
	licenseText); });

	if (subWindow) {
		/*this is a bit of a stupid hack*/
		rendChild = QWidget::createWindowContainer(subWindow, this);
		updateChild();
	}
}

// stupid hack since i cant get some *decent* looking alignment working using qt creator
// also: menuBar dimensions are wrong
// TODO: please someone fix this
void mainWindow::updateChild()
{
	auto geom = this->geometry();

	const auto magicOffset = !fullmodeEditor ? 76 : 0; /*size of menubar returns bullshit*/
	rendChild->setGeometry(QRect(0, magicOffset, geom.width(), geom.height() - magicOffset));
}

void mainWindow::resizeEvent(QResizeEvent* event)
{
	QMainWindow::resizeEvent(event);

	if (rendChild)
		updateChild();
}

void mainWindow::keyPressEvent(QKeyEvent* event)
{
	// toggle tool bar
	if (event->key() == Qt::Key_F11) {
		fullmodeEditor = !fullmodeEditor;

		if (fullmodeEditor) {
			ui->menuBar->hide();
			setWindowState(Qt::WindowFullScreen);
		}
		else {
			setWindowState(Qt::WindowMaximized);
			ui->menuBar->show();
		}
	}
}

void mainWindow::onOpenFile()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select file to load"), "", tr(
		"*"), Q_NULLPTR, QFileDialog::DontResolveSymlinks);

	if (filePath != nullptr) {
		auto qs = QFileInfo(filePath).canonicalFilePath();

		loadFileDialog diag(this);
		if (diag.load(app, qs))
			diag.exec();
	}
}

static bool isValidFile(const QMimeData& md)
{
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

void mainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	if (isValidFile(*event->mimeData()))
		event->accept();
}

void mainWindow::dragMoveEvent(QDragMoveEvent* event)
{
	if (isValidFile(*event->mimeData()))
		event->accept();
}

void mainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
	event->accept();
}

void mainWindow::dropEvent(QDropEvent* event)
{
	auto& md = *event->mimeData();

	if (isValidFile(md)) {
		const auto path = md.urls()[0].toLocalFile();

		loadFileDialog diag(this);
		if (diag.load(app, path))
			diag.exec();
	}
}