
// Copyright (C) 2019-2020 Force67

#ifdef _WIN32
#include <Windows.h>
#endif

#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QMessageBox>
#include <QObject>
#include <QStyleFactory>
#include <QTextDocument>
#include <QTimer>

#include <utl/logger/logger.h>

#include "core.h"

#ifdef _WIN32
#ifdef COMPILING_CORE
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#endif

namespace Solarized {
static const QColor Base03{0, 43, 54};
static const QColor Base02{7, 54, 66};
static const QColor Base01{88, 110, 117};
static const QColor Base00{101, 123, 131};
static const QColor Base0{131, 148, 150};
static const QColor Base1{147, 161, 161};
static const QColor Base2{238, 232, 213};
static const QColor Base3{253, 246, 227};

static const QColor Yellow{181, 137, 0};
static const QColor Orange{203, 75, 22};
static const QColor Red{220, 50, 47};
static const QColor Magenta{211, 54, 130};
static const QColor Violet{108, 113, 196};
static const QColor Blue{38, 139, 210};
static const QColor Cyan{42, 161, 152};
static const QColor Green{133, 153, 0};

static const QColor BaseDark[8]{Base03, Base02, Base01, Base00, Base0, Base1, Base2, Base3};
static const QColor BaseLight[8]{Base3, Base2, Base1, Base0, Base00, Base01, Base02, Base03};
} // namespace Solarized

static QPalette GetSolarizedPalette(bool dark) {
    QPalette p;

    const QColor* base = dark ? Solarized::BaseDark : Solarized::BaseLight;

    p.setColor(QPalette::Base, base[0]);
    p.setColor(QPalette::Window, base[0]);
    p.setColor(QPalette::AlternateBase, base[1]);
    p.setColor(QPalette::Highlight, base[2]);

    p.setColor(QPalette::Text, base[5]);
    p.setColor(QPalette::WindowText, base[5]);
    p.setColor(QPalette::BrightText, base[5]);
    p.setColor(QPalette::ButtonText, base[5]);
    p.setColor(QPalette::PlaceholderText, base[3]);
    p.setColor(QPalette::HighlightedText, base[6]);

    p.setColor(QPalette::Dark, base[0]);
    p.setColor(QPalette::Mid, base[1]);
    p.setColor(QPalette::Button, base[1]);
    p.setColor(QPalette::Midlight, base[2]);
    p.setColor(QPalette::Light, base[3]);

    p.setColor(QPalette::Shadow, Qt::black);

    p.setColor(QPalette::Link, Solarized::Blue);
    p.setColor(QPalette::LinkVisited, Solarized::Red);

    p.setColor(QPalette::NoRole, Solarized::Magenta);

    p.setColor(QPalette::ToolTipBase, base[1]);
    p.setColor(QPalette::ToolTipText, Solarized::Violet);

    return p;
}

static core::FXCore* createApplication(int& argc, char** argv) {
    // core desc - need to be set before creation of
    // qapplication
    QApplication::setApplicationName("FormatX");
    QApplication::setApplicationVersion("1.0");
    QApplication::setApplicationDisplayName("Asset Viewing Utility");
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    auto *app = new core::FXCore(argc, argv);
    app->setStyle("Fusion");
    app->setPalette(GetSolarizedPalette(true));

    QFont f("Verdana");
    f.setPointSize(11);
    app->setFont(f);

    return app;
}

static void applyQtFixes() {
#ifdef _WIN32
    // qt resets our timer resolution - set it back
    using NtQueryTimerResolution_t = LONG(WINAPI*)(PULONG, PULONG, PULONG);
    using NtSetTimerResolution_t = LONG(WINAPI*)(ULONG, BOOLEAN, PULONG);

    const auto hntdll = GetModuleHandleW(L"ntdll.dll");
    auto NtQueryTimerResolution_f = reinterpret_cast<NtQueryTimerResolution_t>(
        GetProcAddress(hntdll, "NtQueryTimerResolution"));
    auto NtSetTimerResolution_f =
        reinterpret_cast<NtSetTimerResolution_t>(GetProcAddress(hntdll, "NtSetTimerResolution"));

    if (NtQueryTimerResolution_f && NtSetTimerResolution_f) {
        ULONG min_res, max_res, orig_res, new_res;
        if (NtQueryTimerResolution_f(&min_res, &max_res, &orig_res) == 0)
            NtSetTimerResolution_f(max_res, TRUE, &new_res);
    }
#endif
}

int CORE_API core_main(int argc, char** argv) {
    utl::createLogger(true);
    LOG_INFO("Initializing FormatX");

    QCommandLineParser parser;
    parser.setApplicationDescription("Welcome to FormatX command line.");
    parser.addPositionalArgument("File", "Path for directly loading a file");
    parser.addPositionalArgument("[Args...]", "Optional args for the executable");

    QScopedPointer<core::FXCore> app(createApplication(argc, argv));
    if (!app->init())
        return 0;

    applyQtFixes();
    return app->exec();
}