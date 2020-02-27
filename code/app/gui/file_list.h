#pragma once

// Copyright (C) 2019-2020 Force67

#include <QAbstractItemModel>
#include <QTreeView>

class fsModel;

// implements an abstracted file view for user defined file systems
class fileView : public QTreeView, public QAbstractTableModel {
public:
    fileView(const char* name);
    ~fileView();

private:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
};