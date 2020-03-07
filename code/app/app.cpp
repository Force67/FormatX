
// Copyright (C) 2019-2020 Force67

#ifdef _WIN32
#include <Windows.h>
#endif

#include <QDirIterator>
#include <QMessageBox>
#include <QWindow>

#include <utl/File.h>
#include <utl/path.h>

#include "app.h"

#include <video_core.h>

// todo: move this
namespace {
class renderWindowQt : public video_core::renderWindow, public QWindow {
public:
    void* getHandle() override;
    void getDimensions(u32& width, u32& height);
};

void* renderWindowQt::getHandle() {
    return reinterpret_cast<void*>(winId());
}

void renderWindowQt::getDimensions(u32& width, u32& height) {
    width = this->width();
    height = this->height();
}
} // namespace

fmtApp::fmtApp(int& argc, char** argv) : QApplication(argc, argv) {
    setApplicationName("FormatX");
    setApplicationVersion("1.0");

    window = std::make_unique<mainWindow>(*this);
}

bool fmtApp::createViewport() {
    // TODO: determine backend based on config
    const auto backendType = video_core::backendKind::dx12;

    static auto outw = std::make_unique<renderWindowQt>();

    renderer = video_core::createRenderer(outw.get(), backendType);

    const char* name;
    switch (backendType) {
    case video_core::backendKind::dx12:
        name = "DirectX 12";
        break;
    case video_core::backendKind::opengl:
        name = "OpenGL";
        break;
    case video_core::backendKind::vulkan:
        name = "Vulkan";
        break;
    case video_core::backendKind::null:
    default:
        name = "Null";
        break;
    }

    // init the renderer
    if (renderer->create()) {
        window->init(nullptr, name);

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