
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

#include "app.h"
#include "main_window.h"
#include "load_file_dialog.h"

#include "qtgen/ui_main_window.h"


mainWindow::mainWindow(fmtApp &app) :
	app(app),
	QMainWindow(nullptr),
	ui(new Ui::main_window())
{
	ui->setupUi(this);
	setAcceptDrops(true);
}

mainWindow::~mainWindow()
{
	delete ui;
}

void mainWindow::init()
{
	connect(ui->openFileAct, &QAction::triggered, this, &mainWindow::onOpenFile);
	show();
}

void mainWindow::onOpenFile()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select file to load"), "", tr(
		"*"), Q_NULLPTR, QFileDialog::DontResolveSymlinks);

	//for this, using 'qs.toUtf8().data() didn't work. 
	//So i changed to get the QByteArray and then get the data. ~Greavesy
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