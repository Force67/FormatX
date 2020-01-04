#pragma once

// Copyright (C) 2019-2020 Force67

#include <QDockWidget>
#include <QDesktopWidget>
#include <QMainWindow>

namespace Ui {
	class main_window;
}

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow();
	~mainWindow();

	void init();
private:
	void createConnects();

	/*callbacks*/
	void onBootSelection();
	void onEnginePause();
	void onEngineStop();

	Ui::main_window* ui;
};