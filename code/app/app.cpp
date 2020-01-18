
// Copyright (C) 2019-2020 Force67

#ifdef _WIN32
#include <Windows.h>
#endif

#include <QDirIterator>
#include <QMessageBox>
#include <utl/path.h>
#include <utl/File.h>

#include "app.h"

fmtApp::fmtApp(int& argc, char** argv) :
	QApplication(argc, argv)
{
	setApplicationName("FormatX");
	setApplicationVersion("1.0");

	window = std::make_unique<mainWindow>(*this);
}

void fmtApp::createWindow()
{
	window->init();
	window->show();
}

bool fmtApp::loadPlugins()
{
	QDirIterator it(QString::fromStdString(utl::make_abs_path("plugins")));
	while (it.hasNext()) {
		auto path = it.next();
		if (path.contains(".dll")) {
			auto* hlib = LoadLibraryW(path.toStdWString().c_str());
			if (hlib) {
				auto* loader = reinterpret_cast<pluginLoader*>(GetProcAddress(hlib, "PLUGIN"));
				if (loader) {
					if (loader->version > PluginVersion::V_1_0) {
						QMessageBox msg(QMessageBox::Critical, tr("Plugin Error!"), tr("Plugin %1 is newer than expected!").arg(path));
						msg.setWindowFlags(msg.windowFlags() & (~Qt::WindowContextHelpButtonHint));
						msg.exec();
						return false;
					}

					plugins.push_back(loader);
				}
				else
					FreeLibrary(hlib);
			}
		}
	}

	return true;
}