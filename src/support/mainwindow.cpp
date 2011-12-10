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
