/*
Authors: Parker Porfilio, Aimei Kutt, Anne Kenyon
*/


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//handle key presses here so I can update the GUI as well.
void MainWindow::keyPressEvent(QKeyEvent *event) {
    GLWidget *mgl = ui->mainGL;
    switch(event->key())
    {
    case Qt::Key_P: {
        if (mgl->m_paused) {
            mgl->setPaused(false);
        } else {
            mgl->setPaused(true);
        }
       //mgl->savePicture();
       break;
    }
    case Qt::Key_Q: {
        mgl->m_camera->cameraMoveUp(true, (event->modifiers() == Qt::ShiftModifier));
        mgl->m_newView = true;
        break;
    }
    case Qt::Key_E: {
        mgl->m_camera->cameraMoveUp(false, (event->modifiers() == Qt::ShiftModifier));
        mgl->m_newView = true;
        break;
    }
    case Qt::Key_W: {
        mgl->m_camera->cameraMoveLook(true, (event->modifiers() == Qt::ShiftModifier));
        mgl->m_newView = true;
        break;
    }
    case Qt::Key_S: {
        mgl->m_camera->cameraMoveLook(false, (event->modifiers() == Qt::ShiftModifier));
        mgl->m_newView = true;
        break;
    }
    case Qt::Key_A: {
        mgl->m_camera->cameraMoveSide(true, (event->modifiers() == Qt::ShiftModifier));
        mgl->m_newView = true;
        break;
    }
    case Qt::Key_D: {
        mgl->m_camera->cameraMoveSide(false, (event->modifiers() == Qt::ShiftModifier));
        mgl->m_newView = true;
        break;
    }
    case Qt::Key_M: {
        ui->ss_button->setChecked(!mgl->global_ss_enabled);
        break;
    }
    case Qt::Key_C: {
        if (mgl->m_camera->getType() == GAME_CAMERA) {
            ui->select_orbit_cam->setChecked(true);
        } else {
            ui->select_game_cam->setChecked(true);
        }
        mgl->m_newView = true;
        break;
    }
    case Qt::Key_L: {
        mgl->loadCamLocation();
        mgl->m_newView = true;
        break;
    }
    case Qt::Key_R: {
        mgl->m_camera->reset();
        mgl->m_newView = true;
        break;
    }
    case Qt::Key_1: {
        mgl->saveCamLocation();
        break;
    }
    case Qt::Key_T: {
        if (mgl->m_show_text) {
            mgl->setShowText(false);
        } else {
            mgl->setShowText(true);
        }
        break;
    }
    case Qt::Key_2: {
        mgl->savePointCloud();
        break;
    }
    case Qt::Key_Z: {
        mgl->m_camera->cameraRotLook(false, (event->modifiers() == Qt::ShiftModifier));
        mgl->m_newView = true;
        break;
    }
    case Qt::Key_X: {
        mgl->m_camera->cameraRotLook(true, (event->modifiers() == Qt::ShiftModifier));
        mgl->m_newView = true;
        break;
    }
    }
}

void MainWindow::on_F_Z3_valueChanged(int value)
{
    ui->mainGL->sliderUpdateF_Z3(value);
}

void MainWindow::on_F_C_x_valueChanged(int value)
{
    ui->mainGL->sliderUpdateF_C_x(value);
}

void MainWindow::on_F_C_y_valueChanged(int value)
{
    ui->mainGL->sliderUpdateF_C_y(value);
}

void MainWindow::on_F_C_z_valueChanged(int value)
{
    ui->mainGL->sliderUpdateF_C_z(value);
}

void MainWindow::on_F_C_w_valueChanged(int value)
{
    ui->mainGL->sliderUpdateF_C_w(value);
}


void MainWindow::on_select_Julia_toggled(bool checked)
{
    ui->mainGL->radioToggeled_Julia(checked);
    ui->select_orbit_cam->setChecked(true);
}

void MainWindow::on_select_Mandelbox_toggled(bool checked)
{
    ui->mainGL->radioToggeled_Mandelbox(checked);
    ui->select_game_cam->setChecked(true);
}


void MainWindow::on_specular_button_toggled(bool checked)
{
    ui->mainGL->radioToggeled_specular(checked);
}

void MainWindow::on_reflection_button_toggled(bool checked)
{
    ui->mainGL->radioToggeled_reflect(checked);
}

void MainWindow::on_skybox_button_toggled(bool checked)
{
    ui->mainGL->checkToggeled_skybox(checked);
}

void MainWindow::on_ss_button_toggled(bool checked)
{
    ui->mainGL->checkToggeled_ss(checked);
}

void MainWindow::on_select_orbit_cam_toggled(bool checked)
{
    ui->mainGL->radioToggeled_Orbit_Cam(checked);
}

void MainWindow::on_select_game_cam_toggled(bool checked)
{
    ui->mainGL->radioToggeled_Game_Cam(checked);
}

void MainWindow::on_reset_camera_button_released()
{
    ui->mainGL->resetCurrentCamera();
}

void MainWindow::on_ITR_slider_valueChanged(int value)
{
    ui->mainGL->sliderUpdate_ITR(value);
}

void MainWindow::on_EPS_slider_valueChanged(int value)
{
    ui->mainGL->sliderUpdate_EPS(value);
}

void MainWindow::on_BRK_slider_valueChanged(int value)
{
    ui->mainGL->sliderUpdate_BRK(value);
}

void MainWindow::on_DEP_slider_valueChanged(int value)
{
    ui->mainGL->sliderUpdate_DEP(value);
}

void MainWindow::on_depth_fog_button_toggled(bool checked)
{
    ui->mainGL->mandelbox_fog = checked;
    ui->mainGL->m_newView = true;
}

void MainWindow::on_coloring_lights_toggled(bool checked)
{
    if (checked) {
        ui->mainGL->mandelbox_coloring = 1;
        ui->mainGL->m_newView = true;
    }
}

void MainWindow::on_coloring_escape_toggled(bool checked)
{
    if (checked) {
        ui->mainGL->mandelbox_coloring = 3;
        ui->mainGL->m_newView = true;
    }
}

void MainWindow::on_coloring_distance_toggled(bool checked)
{
    if (checked) {
        ui->mainGL->mandelbox_coloring = 2;
        ui->mainGL->m_newView = true;
    }
}

void MainWindow::on_select_colorA_toggled(bool __attribute__((unused)) checked)
{
    ui->mainGL->mandelbox_colorScheme = 1;
    ui->mainGL->m_newView = true;
}

void MainWindow::on_select_colorB_toggled(bool checked)
{
    ui->mainGL->mandelbox_colorScheme = 2;
    ui->mainGL->m_newView = true;
}

void MainWindow::on_select_colorC_toggled(bool checked)
{
    ui->mainGL->mandelbox_colorScheme = 3;
    ui->mainGL->m_newView = true;
}

void MainWindow::on_coloring_normals_toggled(bool checked)
{
    if (checked) {
        ui->mainGL->mandelbox_coloring = 4;
        ui->mainGL->m_newView = true;
    }
}

void MainWindow::on_Record_Button_toggled(bool checked)
{
    ui->mainGL->checkRecord(checked);
}
