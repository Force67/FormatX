
// Copyright (C) 2019-2020 Force67

#include <QAction>
#include <QObject>

#include "app.h"
#include "load_file_dialog.h"
#include "qtgen/ui_load_file_dialog.h"

#include <utl/File.h>

loadFileDialog::loadFileDialog(QWidget* parent) : QDialog(parent), ui(new Ui::load_file_dialog()) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint |
                   Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);

    connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(onConfirm()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(onCancel()));
}

loadFileDialog::~loadFileDialog() {
    delete ui;
}

void loadFileDialog::onConfirm() {
    auto loadIndex = ui->pluginLoaderSelectionView->currentIndex();
    if (!loadIndex || loadIndex > loadCanidates.size()) {
        // TODO: log death
        onCancel();
    }

    auto& canidate = loadCanidates[loadIndex];

    // TODO: get rid of conversions
    auto bytes = filePath.toUtf8();
    auto* raw = bytes.constData();

    utl::File file(raw);
    file.Seek(0, utl::seekMode::seek_set);

    // TODO: launch dedicated launching task...
    if (!canidate.loader->init(file, canidate.desc))
        __debugbreak();
}

void loadFileDialog::onCancel() {
    hide();
    destroy();
}

bool loadFileDialog::load(fmtApp& app, const QString& path) {
    filePath = path;

    auto bytes = path.toUtf8();
    auto* raw = bytes.constData();

    utl::File file(raw);
    if (!file.IsOpen())
        return false;

    int acceptibleFiles = 0;
    for (auto* p : app.getPlugins()) {
        file.Seek(0, utl::seekMode::seek_set);

        fileDesc desc{};

        // several plugins might be able to handle this type!
        if (p->accept(file, desc)) {
            ui->pluginLoaderSelectionView->addItem(p->prettyName);

            // TODO: handle this better
            ui->fileTypeCanidatesList->addItem(tr("%1 [%2]").arg(desc.name).arg(p->name));
            acceptibleFiles++;

            loadCanidates.emplace_back(p, desc);
        }
    }

    // failed to find a suitable file loader
    return acceptibleFiles > 0;
}