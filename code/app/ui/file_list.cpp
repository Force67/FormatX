
#include "file_list.h"

#include <QFileSystemModel>

fileView::fileView(const char* name) {
    setModel(this);
    setWindowTitle(name);
    setAnimated(false);
    setSortingEnabled(true);
}

fileView::~fileView() {}

int fileView::rowCount(const QModelIndex& parent /* = QModelIndex() */) const {
    return 1;
}

int fileView::columnCount(const QModelIndex& parent /* = QModelIndex() */) const {
    return 1;
}

QVariant fileView::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const {
    if (role == Qt::DisplayRole)
        return QString("Row%1, Column%2").arg(index.row() + 1).arg(index.column() + 1);

    return QVariant();
}

// QDockWidget;