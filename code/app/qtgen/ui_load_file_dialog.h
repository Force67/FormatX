/********************************************************************************
** Form generated from reading UI file 'load_file_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOAD_FILE_DIALOG_H
#define UI_LOAD_FILE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_load_file_dialog
{
public:
    QListWidget *fileTypeCanidatesList;
    QLabel *itemNameLabel;
    QComboBox *pluginLoaderSelectionView;
    QPushButton *confirmButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *load_file_dialog)
    {
        if (load_file_dialog->objectName().isEmpty())
            load_file_dialog->setObjectName(QString::fromUtf8("load_file_dialog"));
        load_file_dialog->resize(400, 339);
        fileTypeCanidatesList = new QListWidget(load_file_dialog);
        fileTypeCanidatesList->setObjectName(QString::fromUtf8("fileTypeCanidatesList"));
        fileTypeCanidatesList->setGeometry(QRect(20, 50, 361, 192));
        itemNameLabel = new QLabel(load_file_dialog);
        itemNameLabel->setObjectName(QString::fromUtf8("itemNameLabel"));
        itemNameLabel->setGeometry(QRect(20, 20, 69, 20));
        pluginLoaderSelectionView = new QComboBox(load_file_dialog);
        pluginLoaderSelectionView->setObjectName(QString::fromUtf8("pluginLoaderSelectionView"));
        pluginLoaderSelectionView->setGeometry(QRect(20, 260, 361, 28));
        confirmButton = new QPushButton(load_file_dialog);
        confirmButton->setObjectName(QString::fromUtf8("confirmButton"));
        confirmButton->setGeometry(QRect(20, 300, 92, 31));
        cancelButton = new QPushButton(load_file_dialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(120, 300, 92, 31));

        retranslateUi(load_file_dialog);

        QMetaObject::connectSlotsByName(load_file_dialog);
    } // setupUi

    void retranslateUi(QDialog *load_file_dialog)
    {
        load_file_dialog->setWindowTitle(QCoreApplication::translate("load_file_dialog", "Load a new file", nullptr));
        itemNameLabel->setText(QCoreApplication::translate("load_file_dialog", "Load as", nullptr));
        confirmButton->setText(QCoreApplication::translate("load_file_dialog", "Load", nullptr));
        cancelButton->setText(QCoreApplication::translate("load_file_dialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class load_file_dialog: public Ui_load_file_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOAD_FILE_DIALOG_H
