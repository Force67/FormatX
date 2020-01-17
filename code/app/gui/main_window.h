#pragma once

// Copyright (C) 2019-2020 Force67

#include <QDockWidget>
#include <QDesktopWidget>
#include <QMainWindow>

namespace Ui {
	class main_window;
}

class fmtApp;

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow(fmtApp&);
	~mainWindow();

	void init();
private:
	void createConnects();

	/*callbacks*/
	void onOpenFile();

	fmtApp& app;
	Ui::main_window* ui;
};