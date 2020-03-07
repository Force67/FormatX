#pragma once

// Copyright (C) 2019-2020 Force67

#include <QDialog>
#include <QWindow>

#include <plugin_traits.h>

namespace Ui {
class load_file_dialog;
}

class fmtApp;

class loadFileDialog : public QDialog {
    Q_OBJECT

public:
    loadFileDialog(QWidget*);
    ~loadFileDialog();

    bool load(fmtApp&, const QString& path);

private slots:
    void onConfirm();
    void onCancel();

private:
    QString filePath;

    struct pluginInfo {
        pluginLoader* loader;
        fileDesc desc;

        pluginInfo(pluginLoader* l, fileDesc& d)
            : loader(l), desc(d) /*copy intended*/
        {}
    };

    std::vector<pluginInfo> loadCanidates;
    Ui::load_file_dialog* ui;
};