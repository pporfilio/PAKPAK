/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Dec 16 10:14:06 2011
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    GLWidget *mainGL;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QRadioButton *select_Julia;
    QRadioButton *select_Mandelbox;
    QLabel *F_Z3_label;
    QSlider *F_Z3;
    QSpacerItem *verticalSpacer_2;
    QLabel *F_C_Group_label;
    QGridLayout *gridLayout;
    QSlider *F_C_x;
    QSlider *F_C_y;
    QLabel *F_C_x_label;
    QLabel *F_C_y_label;
    QLabel *F_C_z_label;
    QSlider *F_C_z;
    QLabel *F_C_w_label;
    QSlider *F_C_w;
    QSpacerItem *verticalSpacer;
    QWidget *tab_2;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *skybox_button;
    QCheckBox *ss_button;
    QRadioButton *select_orbit_cam;
    QRadioButton *select_game_cam;
    QGridLayout *Specular_Grid;
    QSlider *spec_r;
    QSlider *spec_g;
    QSlider *spec_b;
    QLabel *spec_r_label;
    QLabel *spec_g_label;
    QLabel *spec_b_label;
    QCheckBox *specular_button;
    QGridLayout *gridLayout_2;
    QSlider *reflect_b;
    QSlider *reflect_g;
    QSlider *reflect_r;
    QLabel *reflect_r_label;
    QLabel *reflect_g_label;
    QLabel *reflect_b_label;
    QCheckBox *reflection_button;
    QGridLayout *gridLayout_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        mainGL = new GLWidget(MainWindow);
        mainGL->setObjectName(QString::fromUtf8("mainGL"));
        MainWindow->setCentralWidget(mainGL);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dockWidget->sizePolicy().hasHeightForWidth());
        dockWidget->setSizePolicy(sizePolicy);
        dockWidget->setMinimumSize(QSize(250, 50));
        dockWidget->setBaseSize(QSize(0, 3));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        sizePolicy.setHeightForWidth(dockWidgetContents->sizePolicy().hasHeightForWidth());
        dockWidgetContents->setSizePolicy(sizePolicy);
        dockWidgetContents->setMinimumSize(QSize(220, 0));
        tabWidget = new QTabWidget(dockWidgetContents);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(8, 5, 231, 541));
        tabWidget->setTabPosition(QTabWidget::West);
        tabWidget->setMovable(true);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayoutWidget = new QWidget(tab);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 181, 521));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        select_Julia = new QRadioButton(verticalLayoutWidget);
        select_Julia->setObjectName(QString::fromUtf8("select_Julia"));
        select_Julia->setChecked(true);

        verticalLayout->addWidget(select_Julia);

        select_Mandelbox = new QRadioButton(verticalLayoutWidget);
        select_Mandelbox->setObjectName(QString::fromUtf8("select_Mandelbox"));

        verticalLayout->addWidget(select_Mandelbox);

        F_Z3_label = new QLabel(verticalLayoutWidget);
        F_Z3_label->setObjectName(QString::fromUtf8("F_Z3_label"));

        verticalLayout->addWidget(F_Z3_label);

        F_Z3 = new QSlider(verticalLayoutWidget);
        F_Z3->setObjectName(QString::fromUtf8("F_Z3"));
        F_Z3->setMinimum(-100);
        F_Z3->setMaximum(100);
        F_Z3->setSingleStep(1);
        F_Z3->setValue(0);
        F_Z3->setOrientation(Qt::Horizontal);
        F_Z3->setInvertedAppearance(false);
        F_Z3->setTickPosition(QSlider::NoTicks);

        verticalLayout->addWidget(F_Z3);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        F_C_Group_label = new QLabel(verticalLayoutWidget);
        F_C_Group_label->setObjectName(QString::fromUtf8("F_C_Group_label"));

        verticalLayout->addWidget(F_C_Group_label);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(10, 0, 5, -1);
        F_C_x = new QSlider(verticalLayoutWidget);
        F_C_x->setObjectName(QString::fromUtf8("F_C_x"));
        F_C_x->setMinimum(-100);
        F_C_x->setMaximum(100);
        F_C_x->setValue(-10);
        F_C_x->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(F_C_x, 0, 1, 1, 1);

        F_C_y = new QSlider(verticalLayoutWidget);
        F_C_y->setObjectName(QString::fromUtf8("F_C_y"));
        F_C_y->setMinimum(-100);
        F_C_y->setMaximum(100);
        F_C_y->setValue(10);
        F_C_y->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(F_C_y, 1, 1, 1, 1);

        F_C_x_label = new QLabel(verticalLayoutWidget);
        F_C_x_label->setObjectName(QString::fromUtf8("F_C_x_label"));

        gridLayout->addWidget(F_C_x_label, 0, 0, 1, 1);

        F_C_y_label = new QLabel(verticalLayoutWidget);
        F_C_y_label->setObjectName(QString::fromUtf8("F_C_y_label"));

        gridLayout->addWidget(F_C_y_label, 1, 0, 1, 1);

        F_C_z_label = new QLabel(verticalLayoutWidget);
        F_C_z_label->setObjectName(QString::fromUtf8("F_C_z_label"));

        gridLayout->addWidget(F_C_z_label, 2, 0, 1, 1);

        F_C_z = new QSlider(verticalLayoutWidget);
        F_C_z->setObjectName(QString::fromUtf8("F_C_z"));
        F_C_z->setMinimum(-100);
        F_C_z->setMaximum(100);
        F_C_z->setValue(50);
        F_C_z->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(F_C_z, 2, 1, 1, 1);

        F_C_w_label = new QLabel(verticalLayoutWidget);
        F_C_w_label->setObjectName(QString::fromUtf8("F_C_w_label"));

        gridLayout->addWidget(F_C_w_label, 3, 0, 1, 1);

        F_C_w = new QSlider(verticalLayoutWidget);
        F_C_w->setObjectName(QString::fromUtf8("F_C_w"));
        F_C_w->setMinimum(-100);
        F_C_w->setMaximum(100);
        F_C_w->setValue(-60);
        F_C_w->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(F_C_w, 3, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 250, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayoutWidget_2 = new QWidget(tab_2);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 10, 181, 521));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        skybox_button = new QCheckBox(verticalLayoutWidget_2);
        skybox_button->setObjectName(QString::fromUtf8("skybox_button"));
        skybox_button->setChecked(true);

        verticalLayout_2->addWidget(skybox_button);

        ss_button = new QCheckBox(verticalLayoutWidget_2);
        ss_button->setObjectName(QString::fromUtf8("ss_button"));

        verticalLayout_2->addWidget(ss_button);

        select_orbit_cam = new QRadioButton(verticalLayoutWidget_2);
        select_orbit_cam->setObjectName(QString::fromUtf8("select_orbit_cam"));
        select_orbit_cam->setChecked(true);

        verticalLayout_2->addWidget(select_orbit_cam);

        select_game_cam = new QRadioButton(verticalLayoutWidget_2);
        select_game_cam->setObjectName(QString::fromUtf8("select_game_cam"));

        verticalLayout_2->addWidget(select_game_cam);

        Specular_Grid = new QGridLayout();
        Specular_Grid->setSpacing(6);
        Specular_Grid->setObjectName(QString::fromUtf8("Specular_Grid"));
        Specular_Grid->setContentsMargins(10, -1, 5, -1);
        spec_r = new QSlider(verticalLayoutWidget_2);
        spec_r->setObjectName(QString::fromUtf8("spec_r"));
        spec_r->setMaximum(100);
        spec_r->setValue(50);
        spec_r->setOrientation(Qt::Horizontal);

        Specular_Grid->addWidget(spec_r, 1, 1, 1, 1);

        spec_g = new QSlider(verticalLayoutWidget_2);
        spec_g->setObjectName(QString::fromUtf8("spec_g"));
        spec_g->setMaximum(100);
        spec_g->setValue(50);
        spec_g->setOrientation(Qt::Horizontal);

        Specular_Grid->addWidget(spec_g, 2, 1, 1, 1);

        spec_b = new QSlider(verticalLayoutWidget_2);
        spec_b->setObjectName(QString::fromUtf8("spec_b"));
        spec_b->setMaximum(100);
        spec_b->setValue(100);
        spec_b->setOrientation(Qt::Horizontal);

        Specular_Grid->addWidget(spec_b, 3, 1, 1, 1);

        spec_r_label = new QLabel(verticalLayoutWidget_2);
        spec_r_label->setObjectName(QString::fromUtf8("spec_r_label"));

        Specular_Grid->addWidget(spec_r_label, 1, 0, 1, 1);

        spec_g_label = new QLabel(verticalLayoutWidget_2);
        spec_g_label->setObjectName(QString::fromUtf8("spec_g_label"));

        Specular_Grid->addWidget(spec_g_label, 2, 0, 1, 1);

        spec_b_label = new QLabel(verticalLayoutWidget_2);
        spec_b_label->setObjectName(QString::fromUtf8("spec_b_label"));

        Specular_Grid->addWidget(spec_b_label, 3, 0, 1, 1);

        specular_button = new QCheckBox(verticalLayoutWidget_2);
        specular_button->setObjectName(QString::fromUtf8("specular_button"));
        specular_button->setChecked(true);

        Specular_Grid->addWidget(specular_button, 0, 1, 1, 1);


        verticalLayout_2->addLayout(Specular_Grid);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(10, -1, 5, -1);
        reflect_b = new QSlider(verticalLayoutWidget_2);
        reflect_b->setObjectName(QString::fromUtf8("reflect_b"));
        reflect_b->setMaximum(100);
        reflect_b->setValue(100);
        reflect_b->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(reflect_b, 3, 1, 1, 1);

        reflect_g = new QSlider(verticalLayoutWidget_2);
        reflect_g->setObjectName(QString::fromUtf8("reflect_g"));
        reflect_g->setMaximum(100);
        reflect_g->setValue(100);
        reflect_g->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(reflect_g, 2, 1, 1, 1);

        reflect_r = new QSlider(verticalLayoutWidget_2);
        reflect_r->setObjectName(QString::fromUtf8("reflect_r"));
        reflect_r->setMaximum(100);
        reflect_r->setValue(100);
        reflect_r->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(reflect_r, 1, 1, 1, 1);

        reflect_r_label = new QLabel(verticalLayoutWidget_2);
        reflect_r_label->setObjectName(QString::fromUtf8("reflect_r_label"));

        gridLayout_2->addWidget(reflect_r_label, 1, 0, 1, 1);

        reflect_g_label = new QLabel(verticalLayoutWidget_2);
        reflect_g_label->setObjectName(QString::fromUtf8("reflect_g_label"));

        gridLayout_2->addWidget(reflect_g_label, 2, 0, 1, 1);

        reflect_b_label = new QLabel(verticalLayoutWidget_2);
        reflect_b_label->setObjectName(QString::fromUtf8("reflect_b_label"));

        gridLayout_2->addWidget(reflect_b_label, 3, 0, 1, 1);

        reflection_button = new QCheckBox(verticalLayoutWidget_2);
        reflection_button->setObjectName(QString::fromUtf8("reflection_button"));
        reflection_button->setChecked(true);

        gridLayout_2->addWidget(reflection_button, 0, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout_2);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));

        verticalLayout_2->addLayout(gridLayout_3);

        tabWidget->addTab(tab_2, QString());
        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionQuit);

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "[Lab09] Bloom", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        select_Julia->setText(QApplication::translate("MainWindow", "Julia", 0, QApplication::UnicodeUTF8));
        select_Mandelbox->setText(QApplication::translate("MainWindow", "Mandelbox", 0, QApplication::UnicodeUTF8));
        F_Z3_label->setText(QApplication::translate("MainWindow", "F_Z3 [-1, 1]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        F_Z3->setToolTip(QApplication::translate("MainWindow", "Change F_Z3", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        F_C_Group_label->setText(QApplication::translate("MainWindow", "F_C", 0, QApplication::UnicodeUTF8));
        F_C_x_label->setText(QApplication::translate("MainWindow", "x", 0, QApplication::UnicodeUTF8));
        F_C_y_label->setText(QApplication::translate("MainWindow", "y", 0, QApplication::UnicodeUTF8));
        F_C_z_label->setText(QApplication::translate("MainWindow", "z", 0, QApplication::UnicodeUTF8));
        F_C_w_label->setText(QApplication::translate("MainWindow", "w", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Fractal Parameters", 0, QApplication::UnicodeUTF8));
        skybox_button->setText(QApplication::translate("MainWindow", "Skybox", 0, QApplication::UnicodeUTF8));
        ss_button->setText(QApplication::translate("MainWindow", "Super Sampling", 0, QApplication::UnicodeUTF8));
        select_orbit_cam->setText(QApplication::translate("MainWindow", "Orbit Camera", 0, QApplication::UnicodeUTF8));
        select_game_cam->setText(QApplication::translate("MainWindow", "Game Camera", 0, QApplication::UnicodeUTF8));
        spec_r_label->setText(QApplication::translate("MainWindow", "R", 0, QApplication::UnicodeUTF8));
        spec_g_label->setText(QApplication::translate("MainWindow", "G", 0, QApplication::UnicodeUTF8));
        spec_b_label->setText(QApplication::translate("MainWindow", "B", 0, QApplication::UnicodeUTF8));
        specular_button->setText(QApplication::translate("MainWindow", "Specular", 0, QApplication::UnicodeUTF8));
        reflect_r_label->setText(QApplication::translate("MainWindow", "R", 0, QApplication::UnicodeUTF8));
        reflect_g_label->setText(QApplication::translate("MainWindow", "G", 0, QApplication::UnicodeUTF8));
        reflect_b_label->setText(QApplication::translate("MainWindow", "B", 0, QApplication::UnicodeUTF8));
        reflection_button->setText(QApplication::translate("MainWindow", "Reflection", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Material Properties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
