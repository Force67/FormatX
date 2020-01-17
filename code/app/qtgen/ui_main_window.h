/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_main_window
{
public:
    QAction *openFileAct;
    QAction *showGeneralOptAct;
    QAction *exitAct;
    QAction *aboutAppAct;
    QAction *updateAct;
    QAction *clearRecentAct;
    QAction *freezeRecentAct;
    QWidget *centralWidget;
    QWidget *sizeSliderContainer;
    QHBoxLayout *horizontalLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *openRecentFilesMenu;
    QMenu *menuConfiguration;
    QMenu *menuView;
    QMenu *menuHelp;

    void setupUi(QMainWindow *main_window)
    {
        if (main_window->objectName().isEmpty())
            main_window->setObjectName(QString::fromUtf8("main_window"));
        main_window->resize(1058, 580);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(main_window->sizePolicy().hasHeightForWidth());
        main_window->setSizePolicy(sizePolicy);
        main_window->setMinimumSize(QSize(4, 0));
        main_window->setAutoFillBackground(false);
        main_window->setAnimated(true);
        main_window->setDockNestingEnabled(true);
        main_window->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks|QMainWindow::GroupedDragging);
        openFileAct = new QAction(main_window);
        openFileAct->setObjectName(QString::fromUtf8("openFileAct"));
        showGeneralOptAct = new QAction(main_window);
        showGeneralOptAct->setObjectName(QString::fromUtf8("showGeneralOptAct"));
        exitAct = new QAction(main_window);
        exitAct->setObjectName(QString::fromUtf8("exitAct"));
        aboutAppAct = new QAction(main_window);
        aboutAppAct->setObjectName(QString::fromUtf8("aboutAppAct"));
        updateAct = new QAction(main_window);
        updateAct->setObjectName(QString::fromUtf8("updateAct"));
        clearRecentAct = new QAction(main_window);
        clearRecentAct->setObjectName(QString::fromUtf8("clearRecentAct"));
        freezeRecentAct = new QAction(main_window);
        freezeRecentAct->setObjectName(QString::fromUtf8("freezeRecentAct"));
        freezeRecentAct->setCheckable(true);
        centralWidget = new QWidget(main_window);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setLocale(QLocale(QLocale::English, QLocale::Germany));
        sizeSliderContainer = new QWidget(centralWidget);
        sizeSliderContainer->setObjectName(QString::fromUtf8("sizeSliderContainer"));
        sizeSliderContainer->setGeometry(QRect(280, 10, 181, 31));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sizeSliderContainer->sizePolicy().hasHeightForWidth());
        sizeSliderContainer->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(sizeSliderContainer);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(14, 0, 14, 0);
        main_window->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(main_window);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1058, 18));
        menuBar->setContextMenuPolicy(Qt::PreventContextMenu);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        openRecentFilesMenu = new QMenu(menuFile);
        openRecentFilesMenu->setObjectName(QString::fromUtf8("openRecentFilesMenu"));
        openRecentFilesMenu->setEnabled(true);
        openRecentFilesMenu->setToolTipsVisible(true);
        menuConfiguration = new QMenu(menuBar);
        menuConfiguration->setObjectName(QString::fromUtf8("menuConfiguration"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        main_window->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuConfiguration->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(openFileAct);
        menuFile->addAction(openRecentFilesMenu->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(exitAct);
        openRecentFilesMenu->addAction(clearRecentAct);
        openRecentFilesMenu->addAction(freezeRecentAct);
        openRecentFilesMenu->addSeparator();
        menuConfiguration->addAction(showGeneralOptAct);
        menuHelp->addAction(updateAct);
        menuHelp->addSeparator();
        menuHelp->addAction(aboutAppAct);

        retranslateUi(main_window);

        QMetaObject::connectSlotsByName(main_window);
    } // setupUi

    void retranslateUi(QMainWindow *main_window)
    {
        main_window->setWindowTitle(QCoreApplication::translate("main_window", "FormatX", nullptr));
        openFileAct->setText(QCoreApplication::translate("main_window", "Open File", nullptr));
        showGeneralOptAct->setText(QCoreApplication::translate("main_window", "General", nullptr));
#if QT_CONFIG(tooltip)
        showGeneralOptAct->setToolTip(QCoreApplication::translate("main_window", "Configure CPU", nullptr));
#endif // QT_CONFIG(tooltip)
        exitAct->setText(QCoreApplication::translate("main_window", "Exit", nullptr));
#if QT_CONFIG(tooltip)
        exitAct->setToolTip(QCoreApplication::translate("main_window", "Exit RPCS3", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        exitAct->setStatusTip(QCoreApplication::translate("main_window", "Exit the application", nullptr));
#endif // QT_CONFIG(statustip)
        aboutAppAct->setText(QCoreApplication::translate("main_window", "About FormatX", nullptr));
        updateAct->setText(QCoreApplication::translate("main_window", "Check for Updates", nullptr));
        clearRecentAct->setText(QCoreApplication::translate("main_window", "List Clear", nullptr));
        freezeRecentAct->setText(QCoreApplication::translate("main_window", "List Freeze", nullptr));
        menuFile->setTitle(QCoreApplication::translate("main_window", "File", nullptr));
        openRecentFilesMenu->setTitle(QCoreApplication::translate("main_window", "Open Recent Files", nullptr));
        menuConfiguration->setTitle(QCoreApplication::translate("main_window", "Options", nullptr));
        menuView->setTitle(QCoreApplication::translate("main_window", "View", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("main_window", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class main_window: public Ui_main_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
