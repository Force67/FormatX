#pragma once

// Copyright (C) 2019-2020 Force67

#include <QApplication>
#include <QFontDatabase>
#include <QIcon>
#include <QObject>

#include <plugintraits.h>

#include "gui/main_window.h"

class fmtApp : public QApplication
{
	Q_OBJECT
public:
	using pluginList = std::vector<pluginLoader*>;

	fmtApp(int&, char**);

	bool loadPlugins();
	void createWindow();

	void loadFile(const char* name);
	
	pluginList& getPlugins() {
		return plugins;
	}

private:
	pluginList plugins;
	std::unique_ptr<mainWindow> window;
};