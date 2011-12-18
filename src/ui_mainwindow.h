/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat Dec 17 19:43:55 2011
**      by: Qt User Interface Compiler version 4.6.3
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
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
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
    QGroupBox *groupBox_2;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_6;
    QRadioButton *select_Julia;
    QRadioButton *select_Mandelbox;
    QGroupBox *groupBox;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_5;
    QRadioButton *select_orbit_cam;
    QRadioButton *select_game_cam;
    QPushButton *reset_camera_button;
    QFrame *line_2;
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
    QGridLayout *gridLayout_4;
    QSlider *ITR_slider;
    QSlider *EPS_slider;
    QSlider *BRK_slider;
    QLabel *ITR_label;
    QLabel *EPS_label;
    QLabel *BRK_label;
    QSlider *DEP_slider;
    QLabel *DEP_label;
    QWidget *tab_2;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *skybox_button;
    QCheckBox *ss_button;
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
    QCheckBox *depth_fog_button;
    QRadioButton *coloring_lights;
    QRadioButton *coloring_distance;
    QLabel *label;

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
        menuBar->setGeometry(QRect(0, 0, 800, 23));
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
        groupBox_2 = new QGroupBox(verticalLayoutWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(0, 60));
        verticalLayoutWidget_4 = new QWidget(groupBox_2);
        verticalLayoutWidget_4->setObjectName(QString::fromUtf8("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(10, 0, 161, 52));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        select_Julia = new QRadioButton(verticalLayoutWidget_4);
        select_Julia->setObjectName(QString::fromUtf8("select_Julia"));
        select_Julia->setChecked(true);

        verticalLayout_6->addWidget(select_Julia);

        select_Mandelbox = new QRadioButton(verticalLayoutWidget_4);
        select_Mandelbox->setObjectName(QString::fromUtf8("select_Mandelbox"));

        verticalLayout_6->addWidget(select_Mandelbox);


        verticalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(verticalLayoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 60));
        verticalLayoutWidget_3 = new QWidget(groupBox);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(10, 0, 160, 52));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        select_orbit_cam = new QRadioButton(verticalLayoutWidget_3);
        select_orbit_cam->setObjectName(QString::fromUtf8("select_orbit_cam"));
        select_orbit_cam->setChecked(true);

        verticalLayout_5->addWidget(select_orbit_cam);

        select_game_cam = new QRadioButton(verticalLayoutWidget_3);
        select_game_cam->setObjectName(QString::fromUtf8("select_game_cam"));

        verticalLayout_5->addWidget(select_game_cam);


        verticalLayout->addWidget(groupBox);

        reset_camera_button = new QPushButton(verticalLayoutWidget);
        reset_camera_button->setObjectName(QString::fromUtf8("reset_camera_button"));

        verticalLayout->addWidget(reset_camera_button);

        line_2 = new QFrame(verticalLayoutWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

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

        verticalSpacer = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(5, 0, 5, -1);
        ITR_slider = new QSlider(verticalLayoutWidget);
        ITR_slider->setObjectName(QString::fromUtf8("ITR_slider"));
        ITR_slider->setMinimum(100);
        ITR_slider->setMaximum(1000);
        ITR_slider->setSingleStep(10);
        ITR_slider->setPageStep(100);
        ITR_slider->setValue(300);
        ITR_slider->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(ITR_slider, 0, 1, 1, 1);

        EPS_slider = new QSlider(verticalLayoutWidget);
        EPS_slider->setObjectName(QString::fromUtf8("EPS_slider"));
        EPS_slider->setMinimum(1);
        EPS_slider->setMaximum(10000);
        EPS_slider->setSingleStep(100);
        EPS_slider->setPageStep(1000);
        EPS_slider->setValue(3000);
        EPS_slider->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(EPS_slider, 1, 1, 1, 1);

        BRK_slider = new QSlider(verticalLayoutWidget);
        BRK_slider->setObjectName(QString::fromUtf8("BRK_slider"));
        BRK_slider->setMinimum(10);
        BRK_slider->setMaximum(500);
        BRK_slider->setValue(200);
        BRK_slider->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(BRK_slider, 2, 1, 1, 1);

        ITR_label = new QLabel(verticalLayoutWidget);
        ITR_label->setObjectName(QString::fromUtf8("ITR_label"));

        gridLayout_4->addWidget(ITR_label, 0, 0, 1, 1);

        EPS_label = new QLabel(verticalLayoutWidget);
        EPS_label->setObjectName(QString::fromUtf8("EPS_label"));

        gridLayout_4->addWidget(EPS_label, 1, 0, 1, 1);

        BRK_label = new QLabel(verticalLayoutWidget);
        BRK_label->setObjectName(QString::fromUtf8("BRK_label"));

        gridLayout_4->addWidget(BRK_label, 2, 0, 1, 1);

        DEP_slider = new QSlider(verticalLayoutWidget);
        DEP_slider->setObjectName(QString::fromUtf8("DEP_slider"));
        DEP_slider->setMinimum(1);
        DEP_slider->setMaximum(100);
        DEP_slider->setValue(10);
        DEP_slider->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(DEP_slider, 3, 1, 1, 1);

        DEP_label = new QLabel(verticalLayoutWidget);
        DEP_label->setObjectName(QString::fromUtf8("DEP_label"));

        gridLayout_4->addWidget(DEP_label, 3, 0, 1, 1);


        verticalLayout->addLayout(gridLayout_4);

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
        ss_button->setChecked(true);

        verticalLayout_2->addWidget(ss_button);

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
        depth_fog_button = new QCheckBox(verticalLayoutWidget_2);
        depth_fog_button->setObjectName(QString::fromUtf8("depth_fog_button"));
        depth_fog_button->setChecked(true);

        gridLayout_3->addWidget(depth_fog_button, 0, 0, 1, 1);

        coloring_lights = new QRadioButton(verticalLayoutWidget_2);
        coloring_lights->setObjectName(QString::fromUtf8("coloring_lights"));
        coloring_lights->setChecked(true);

        gridLayout_3->addWidget(coloring_lights, 2, 0, 1, 1);

        coloring_distance = new QRadioButton(verticalLayoutWidget_2);
        coloring_distance->setObjectName(QString::fromUtf8("coloring_distance"));

        gridLayout_3->addWidget(coloring_distance, 3, 0, 1, 1);

        label = new QLabel(verticalLayoutWidget_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        gridLayout_3->addWidget(label, 1, 0, 1, 1);


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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Fractal", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
        select_Julia->setText(QApplication::translate("MainWindow", "Julia", 0, QApplication::UnicodeUTF8));
        select_Mandelbox->setText(QApplication::translate("MainWindow", "Mandelbox", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QString());
        select_orbit_cam->setText(QApplication::translate("MainWindow", "Orbit Camera", 0, QApplication::UnicodeUTF8));
        select_game_cam->setText(QApplication::translate("MainWindow", "Game Camera", 0, QApplication::UnicodeUTF8));
        reset_camera_button->setText(QApplication::translate("MainWindow", "Reset Camera Location", 0, QApplication::UnicodeUTF8));
        F_C_Group_label->setText(QApplication::translate("MainWindow", "F_C", 0, QApplication::UnicodeUTF8));
        F_C_x_label->setText(QApplication::translate("MainWindow", "x", 0, QApplication::UnicodeUTF8));
        F_C_y_label->setText(QApplication::translate("MainWindow", "y", 0, QApplication::UnicodeUTF8));
        F_C_z_label->setText(QApplication::translate("MainWindow", "z", 0, QApplication::UnicodeUTF8));
        F_C_w_label->setText(QApplication::translate("MainWindow", "w", 0, QApplication::UnicodeUTF8));
        ITR_label->setText(QApplication::translate("MainWindow", "ITR", 0, QApplication::UnicodeUTF8));
        EPS_label->setText(QApplication::translate("MainWindow", "EPS", 0, QApplication::UnicodeUTF8));
        BRK_label->setText(QApplication::translate("MainWindow", "BRK", 0, QApplication::UnicodeUTF8));
        DEP_label->setText(QApplication::translate("MainWindow", "DEP", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Fractal Parameters", 0, QApplication::UnicodeUTF8));
        skybox_button->setText(QApplication::translate("MainWindow", "Skybox", 0, QApplication::UnicodeUTF8));
        ss_button->setText(QApplication::translate("MainWindow", "Super Sampling", 0, QApplication::UnicodeUTF8));
        spec_r_label->setText(QApplication::translate("MainWindow", "R", 0, QApplication::UnicodeUTF8));
        spec_g_label->setText(QApplication::translate("MainWindow", "G", 0, QApplication::UnicodeUTF8));
        spec_b_label->setText(QApplication::translate("MainWindow", "B", 0, QApplication::UnicodeUTF8));
        specular_button->setText(QApplication::translate("MainWindow", "Specular", 0, QApplication::UnicodeUTF8));
        reflect_r_label->setText(QApplication::translate("MainWindow", "R", 0, QApplication::UnicodeUTF8));
        reflect_g_label->setText(QApplication::translate("MainWindow", "G", 0, QApplication::UnicodeUTF8));
        reflect_b_label->setText(QApplication::translate("MainWindow", "B", 0, QApplication::UnicodeUTF8));
        reflection_button->setText(QApplication::translate("MainWindow", "Reflection", 0, QApplication::UnicodeUTF8));
        depth_fog_button->setText(QApplication::translate("MainWindow", "Depth Fog", 0, QApplication::UnicodeUTF8));
        coloring_lights->setText(QApplication::translate("MainWindow", "lights", 0, QApplication::UnicodeUTF8));
        coloring_distance->setText(QApplication::translate("MainWindow", "distance", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Coloring Options", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Material Properties", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
