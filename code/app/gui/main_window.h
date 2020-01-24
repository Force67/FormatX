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

	void init(QWindow *w = nullptr);
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
	QWidget* rendChild = nullptr;
	Ui::main_window* ui;

	bool fullmodeEditor = false;
	QRect backupGeom{};
};