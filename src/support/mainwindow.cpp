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

void MainWindow::on_enable_specular_reflection_stateChanged(int button_val)
{
    //ui->mainGL->spec_reflect_box_changed(button_val);
}

void MainWindow::on_enable_normal_reflection_stateChanged(int button_val)
{
    //ui->mainGL->normal_reflect_box_changed(button_val);
}

void MainWindow::on_spec_r_valueChanged(int value)
{
    //ui->mainGL->sli
}

void MainWindow::on_spec_g_valueChanged(int value)
{

}

void MainWindow::on_spec_b_valueChanged(int value)
{

}

void MainWindow::on_reflect_r_valueChanged(int value)
{

}

void MainWindow::on_reflect_g_valueChanged(int value)
{

}

void MainWindow::on_reflect_b_valueChanged(int value)
{

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
