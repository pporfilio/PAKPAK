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
    ui->select_Mandelbox->setChecked(false);
    ui->mainGL->radioToggeled_Mandelbox(false);
}

void MainWindow::on_select_Mandelbox_toggled(bool checked)
{
    ui->mainGL->radioToggeled_Mandelbox(checked);
    ui->select_Julia->setChecked(false);
    ui->mainGL->radioToggeled_Julia(false);
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
