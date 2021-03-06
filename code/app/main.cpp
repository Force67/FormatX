
// Copyright (C) 2019-2020 Force67

#ifdef _WIN32
#include <Windows.h>
#endif

#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QTimer>
#include <QObject>
#include <QMessageBox>
#include <QTextDocument>
#include <QStyleFactory>

#include "app.h"

int main(int argc, char** argv)
{
	qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QCommandLineParser parser;
	parser.setApplicationDescription("Welcome to formatx command line.");
	parser.addPositionalArgument("File", "Path for directly loading a file");
	parser.addPositionalArgument("[Args...]", "Optional args for the executable");

	//TODO: parse positional arguments
	QScopedPointer<fmtApp> appInstance(new fmtApp(argc, argv));
	if (!appInstance->loadPlugins())
		return 0;

#ifdef _WIN32
	// fix for timer resolution
	{
		using NtQueryTimerResolution_t = LONG(WINAPI*)(PULONG, PULONG, PULONG);
		using NtSetTimerResolution_t = LONG(WINAPI*)(ULONG, BOOLEAN, PULONG);

		auto hNtLib = GetModuleHandleW(L"ntdll.dll");
		auto NtQueryTimerResolution_f = reinterpret_cast<NtQueryTimerResolution_t>(GetProcAddress(hNtLib, "NtQueryTimerResolution"));
		auto NtSetTimerResolution_f = reinterpret_cast<NtSetTimerResolution_t>(GetProcAddress(hNtLib, "NtSetTimerResolution"));

		ULONG min_res, max_res, orig_res, new_res;
		if (NtQueryTimerResolution_f(&min_res, &max_res, &orig_res) == 0)
			NtSetTimerResolution_f(max_res, TRUE, &new_res);
	}
#endif

	appInstance->createWindow();

	int res = appInstance->exec();

	//TODO: cleanup some stuff?

	return res;
}