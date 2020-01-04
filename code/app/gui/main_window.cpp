
// Copyright (C) 2019-2020 Force67

#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QDesktopWidget>
#include <QMimeData>
#include <QScreen>

#include "app.h"
#include "main_window.h"
#include "qtgen/ui_main_window.h"


mainWindow::mainWindow(fmtApp &app) :
	app(app),
	QMainWindow(nullptr),
	ui(new Ui::main_window())
{}

mainWindow::~mainWindow()
{
	delete ui;
}

void mainWindow::init()
{
	ui->setupUi(this);
	createConnects();

	setAcceptDrops(true);
	show();
}

void mainWindow::onOpenFile()
{
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select file to load"), "", tr(
		"*"), Q_NULLPTR, QFileDialog::DontResolveSymlinks);

	if (filePath != nullptr) {
		auto qs = QFileInfo(filePath).canonicalFilePath();
		auto* raw = qs.toUtf8().constData();
		app.loadFile(raw);
	}
}

void mainWindow::createConnects()
{
	connect(ui->openFileAct, &QAction::triggered, this, &mainWindow::onOpenFile);

	//connect(ui->bootAct, &QAction::triggered, this, &mainWindow::onBootSelection);
	//connect(ui->enginePauseAct, &QAction::triggered, this, &mainWindow::onEnginePause);
	//connect(ui->engineStopAct, &QAction::triggered, this, &mainWindow::onEnginePause);
}