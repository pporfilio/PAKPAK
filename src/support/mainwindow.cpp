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
       mgl->savePicture();
       break;
    }
    case Qt::Key_Minus: {
        mgl->F_Z3 = max(-1, mgl->F_Z3 - .05);
        break;
    }
    case Qt::Key_Equal: {
        mgl->F_Z3 = min(1, mgl->F_Z3 + .05);
        break;
    }
    case Qt::Key_Q: {
        mgl->m_camera->cameraMoveUp(true, (event->modifiers() == Qt::ShiftModifier));
        break;
    }
    case Qt::Key_E: {
        mgl->m_camera->cameraMoveUp(false, (event->modifiers() == Qt::ShiftModifier));
        break;
    }
    case Qt::Key_W: {
        mgl->m_camera->cameraMoveLook(true, (event->modifiers() == Qt::ShiftModifier));
        break;
    }
    case Qt::Key_S: {
        mgl->m_camera->cameraMoveLook(false, (event->modifiers() == Qt::ShiftModifier));
        break;
    }
    case Qt::Key_A: {
        mgl->m_camera->cameraMoveSide(true, (event->modifiers() == Qt::ShiftModifier));
        break;
    }
    case Qt::Key_D: {
        mgl->m_camera->cameraMoveSide(false, (event->modifiers() == Qt::ShiftModifier));
        break;
    }
    case Qt::Key_M: {
        mgl->global_ss_enabled = !mgl->global_ss_enabled;
        mgl->ss_enabled = mgl->global_ss_enabled;
        break;
    }
    case Qt::Key_C: {
        if (mgl->m_camera->getType() == GAME_CAMERA) {
            mgl->m_camera = mgl->m_orbitCamera;
        } else {
            mgl->m_camera = mgl->m_gameCamera;
        }
        break;
    }
    case Qt::Key_L: {
        mgl->loadCamLocation();
        break;
    }
    case Qt::Key_R: {
        mgl->m_camera->reset();
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



void MainWindow::on_spec_r_valueChanged(int value)
{
    ui->mainGL->sliderUpdateF_spec_channels_r(value);
}

void MainWindow::on_spec_g_valueChanged(int value)
{
    ui->mainGL->sliderUpdateF_spec_channels_g(value);
}

void MainWindow::on_spec_b_valueChanged(int value)
{
    ui->mainGL->sliderUpdateF_spec_channels_b(value);
}



void MainWindow::on_reflect_r_valueChanged(int value)
{
    ui->mainGL->sliderUpdateF_reflect_channels_r(value);
}

void MainWindow::on_reflect_g_valueChanged(int value)
{
    ui->mainGL->sliderUpdateF_reflect_channels_r(value);
}

void MainWindow::on_reflect_b_valueChanged(int value)
{
    ui->mainGL->sliderUpdateF_reflect_channels_r(value);
}



void MainWindow::on_select_Julia_toggled(bool checked)
{
    ui->mainGL->radioToggeled_Julia(checked);
//    ui->select_Mandelbox->setChecked(false);
//    ui->mainGL->radioToggeled_Mandelbox(false);
    ui->select_orbit_cam->setChecked(true);
}

void MainWindow::on_select_Mandelbox_toggled(bool checked)
{
    ui->mainGL->radioToggeled_Mandelbox(checked);
//    ui->select_Julia->setChecked(false);
//    ui->mainGL->radioToggeled_Julia(false);
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
