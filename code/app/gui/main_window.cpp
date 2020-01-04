
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

#include "main_window.h"
#include "qtgen/ui_main_window.h"


mainWindow::mainWindow() :
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

void mainWindow::onBootSelection()
{
	// TODO: pause system
	QString filePath = QFileDialog::getOpenFileName(this, tr("Select (S)ELF to boot"), "", tr(
		"(S)ELF files (*BOOT.BIN *.elf *.self);;"
		"ELF files (BOOT.BIN *.elf);;"
		"SELF files (EBOOT.BIN *.self);;"
		"BOOT files (*BOOT.BIN);"
		"BIN files (*.bin);;",
		Q_NULLPTR, QFileDialog::DontResolveSymlinks));

	if (filePath == nullptr) {
		// resume system
	}

	auto qs = QFileInfo(filePath).canonicalFilePath();
	auto* strData = qs.toUtf8().constData();
}

void mainWindow::onEnginePause()
{

}

void mainWindow::onEngineStop()
{
}

void mainWindow::createConnects()
{
	//connect(ui->bootAct, &QAction::triggered, this, &mainWindow::onBootSelection);
	//connect(ui->enginePauseAct, &QAction::triggered, this, &mainWindow::onEnginePause);
	//connect(ui->engineStopAct, &QAction::triggered, this, &mainWindow::onEnginePause);
}