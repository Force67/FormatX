
// Copyright (C) 2019-2020 Force67

#ifdef _WIN32
#include <Windows.h>
#endif

#include <QDirIterator>
#include <QMessageBox>
#include <utl/File.h>
#include <utl/path.h>

#include "app.h"

#include "rend/vk/vk_rend.h"

fmtApp::fmtApp(int& argc, char** argv) : QApplication(argc, argv) {
    setApplicationName("FormatX");
    setApplicationVersion("1.0");

    window = std::make_unique<mainWindow>(*this);
}

bool fmtApp::createViewport() {
    // TODO: determine backend based on config
    const auto backendType = rend::backendKind::vulkan;

    renderer = rend::createRenderer(nullptr, backendType);

    // special wrap feature for VK
    QWindow* wrapWindow = backendType == rend::backendKind::vulkan
                          ? reinterpret_cast<rend::vkBackend*>(renderer.get())
                          : nullptr;

    const char* name;
    switch (backendType) {
    case rend::backendKind::dx12:
        name = "DirectX 12";
        break;
    case rend::backendKind::opengl:
        name = "OpenGL";
        break;
    case rend::backendKind::vulkan:
        name = "Vulkan";
        break;
    case rend::backendKind::null:
    default:
        name = "Null";
        break;
    }

    // init the renderer
    if (renderer->create()) {
        window->init(wrapWindow, name);

        LOG_INFO("Initializing {} renderer", name);

        window->show();
        return true;
    }

    LOG_ERROR("Failed to create a renderinterface for {}", name);
    return false;
}

bool fmtApp::loadPlugins() {
    QDirIterator it(QString::fromStdString(utl::make_abs_path("plugins")));
    while (it.hasNext()) {
        auto path = it.next();
        if (path.contains(".dll")) {
            if (auto* hlib = LoadLibraryW(path.toStdWString().c_str())) {
                if (auto* loader =
                        reinterpret_cast<pluginLoader*>(GetProcAddress(hlib, "PLUGIN"))) {
                    if (loader->version > PluginVersion::V_1_0) {
                        QMessageBox msg(QMessageBox::Critical, tr("Plugin Error!"),
                                        tr("Plugin %1 is newer than expected!").arg(path));
                        msg.setWindowFlags(msg.windowFlags() & (~Qt::WindowContextHelpButtonHint));
                        msg.exec();
                        return false;
                    }

                    plugins.push_back(loader);
                } else
                    FreeLibrary(hlib);
            }
        }
    }

    return true;
}